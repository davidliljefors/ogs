#include "Core.h"
#include "GLContext.h"


class Game : public ogs::GLContext
{

	void OnUpdate(float) override
	{
		if (GetKey(GLFW_KEY_LEFT).held)
		{
			ogs::LogHint("Pressed button");
		}

	};
};

int main()
{
	ogs::GLContext ctx( { 1600, 900 });
	ctx.Run();

	return 0;
}
