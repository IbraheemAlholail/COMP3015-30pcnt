#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

layout (binding = 0) uniform sampler2D glassTex;
layout (binding = 1) uniform sampler2D woodTex;
layout (binding = 2) uniform sampler2D cementTex;
layout (binding = 3) uniform sampler2D fishTex;

layout (location = 0) out vec4 FragColor;

uniform struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 L;
} Light;

uniform struct MaterialInfo {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
    int texChoice;
} Material;

uniform struct SpotLightInfo{
	vec3 Position;
	vec3 La;
	vec3 L;
	vec3 Direction;
	float Exponent;
	float Cutoff;
} Spot;

const int levels = 5;
const float scaleFactor = 1.0 / levels;


vec3 blinnPhong(vec3 position, vec3 n) {
    vec3 diffuse = vec3(0.0), spec = vec3(0.0);

    vec3 glassTexColor = texture(glassTex, TexCoord).rgb;
    vec3 woodTexColor = texture(woodTex, TexCoord).rgb;
	vec3 cementTexColor = texture(cementTex, TexCoord).rgb;
	vec3 fishTexColor = texture(fishTex, TexCoord).rgb;
    vec3 texColor;
    
    if (Material.texChoice == 0) {
		texColor = glassTexColor;
	} else if(Material.texChoice == 1){
		texColor = woodTexColor;
	}else if (Material.texChoice == 3){
		texColor = vec3(0.0);
	}else if (Material.texChoice == 4){
		texColor = cementTexColor;
	}else if (Material.texChoice == 5){
	    texColor = fishTexColor;
	}


    vec3 ambient =  Light.La * texColor;
    vec3 s = normalize(Light.Position.xyz - position);

    float sDotn = max(dot(s, n), 0.0);
    diffuse = texColor * sDotn;

    if (sDotn > 0.0) {
        vec3 v = normalize(-position.xyz);
        vec3 h = normalize(s + v);
        spec = Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess);
    }
    
    return ambient + (diffuse + spec) * Light.L;
}

vec3 blinnphongSpot(vec3 position, vec3 n){

	vec3 diffuse = vec3(0), spec = vec3(0);
	vec3 ambient = Spot.La * Material.Ka;

	vec3 s = normalize(Spot.Position - position);

	float cosAng = dot(-s , normalize(Spot.Direction));
	float angle = acos(cosAng);
	float spotScale;

	if(angle >= 0.0 && angle < Spot.Cutoff){
		spotScale = pow(cosAng, Spot.Exponent);
		float sDotN = max( dot(s,n), 0.0);
		diffuse = Material.Kd * floor(sDotN*levels) * scaleFactor;

		if (sDotN > 0.0){
			vec3 v = normalize(-position.xyz);
			vec3 h = normalize(v + s);
			spec = Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess);
		}
	}

	return ambient + spotScale * (diffuse + spec) * Spot.L ;
}

void main() {
    vec3 lightyThing = vec3 (0.0);
	lightyThing += blinnPhong(Position, normalize(Normal));
	lightyThing += blinnphongSpot(Position, normalize(Normal));
	FragColor = vec4(lightyThing, 1.0);
}