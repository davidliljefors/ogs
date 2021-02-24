#include "ogspch.h"
#include "Material.h"


ogs::Material::Material()
{
	_diffuse = ogs::Texture::GetWhiteTexture();
	_specular = ogs::Texture::GetWhiteTexture();
}

ogs::Material::Material(Texture const* diffuse, Texture const* specular)
{
	_diffuse = diffuse;
	_specular = specular;
}

void ogs::Material::Bind(ogs::Shader const& shader) const
{
	shader.SetFloat("u_Material.shininess", _shininess);
	_diffuse->Bind(0);
	shader.SetInt("u_Material.diffuse", 0);
	_specular->Bind(1);
	shader.SetInt("u_Material.specular", 1);

}
