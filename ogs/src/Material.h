#pragma once


namespace ogs {

class Shader;
class Texture;

class Material
{
public:
	Material();
	Material(Texture const* diffuse, Texture const* specular);
	void Bind(ogs::Shader const& shader) const;


private:
	Texture const* _diffuse;
	Texture const* _specular;
	float _shininess = 32.0F;
};

}