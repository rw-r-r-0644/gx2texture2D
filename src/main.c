#include <gx2/texture.h>
#include <gx2/mem.h>
#include <whb/gfx.h>
#include <whb/proc.h>
#include <romfs-wiiu.h>

#include <malloc.h>
#include <stdbool.h>

#include "memory.h"
#include "model.h"
#include "gx2_ext.h"
#include "draw.h"

float box_x = 512.0f;

GX2Texture texture;
WHBGfxShaderGroup shaderGroup;

void setup_scene(void)
{
	// Load the shader
	void *gshFileData = read_file_to_bytes("romfs:/textureShader.gsh");

	WHBGfxLoadGFDShaderGroup(&shaderGroup, 0, gshFileData);
	WHBGfxInitShaderAttribute(&shaderGroup, "aPosition", 0, 0, GX2_ATTRIB_FORMAT_FLOAT_32_32_32);
	WHBGfxInitShaderAttribute(&shaderGroup, "aTexCoord", 1, 0, GX2_ATTRIB_FORMAT_FLOAT_32_32);
	WHBGfxInitShaderAttribute(&shaderGroup, "aColour", 2, 0, GX2_ATTRIB_FORMAT_FLOAT_32_32_32_32);
	WHBGfxInitFetchShader(&shaderGroup);

	free(gshFileData);

	// Load the texture
	GX2InitTexture(&texture, 128, 128, 1, 0, GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8, GX2_SURFACE_DIM_TEXTURE_2D, GX2_TILE_MODE_LINEAR_ALIGNED);
	texture.surface.image = memalign(texture.surface.alignment, texture.surface.imageSize);
	memcpy(texture.surface.image, test_image, texture.surface.imageSize);
	GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_TEXTURE, texture.surface.image, texture.surface.imageSize);
}

void takedown_scene()
{
	// Free texture memory
	free(texture.surface.image);
}

void prepare_render()
{
	GX2SetAlphaTest(true, GX2_COMPARE_FUNC_GREATER, 0.0f);
	GX2SetDepthOnlyControl(false, false, GX2_COMPARE_FUNC_NEVER);
	GX2SetColorControl(GX2_LOGIC_OP_COPY, 0xFF, false, true);
	GX2SetBlendControl(GX2_RENDER_TARGET_0, GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_COMBINE_MODE_ADD, true, GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_COMBINE_MODE_ADD);
	GX2SetCullOnlyControl(GX2_FRONT_FACE_CCW, false, false);
}

//This is our actual rendering code
void render()
{
	clean_tex_allocs();
	
	box_x += 10.0f;
	if(box_x > (float)TARGET_WIDTH)
		box_x = -512.0f;
	
	render_texture(&texture, box_x, 512.0f, 512.0f, 512.0f);
	render_texture_color(&texture, 200.0f, 200.0f, 512.0f, 512.0f, 0.5f, 1.0f, 0.0f, 0.8f);
	render_texture_color(&texture, 50.0f, 50.0f, 512.0f, 512.0f, 0.0f, 0.5f, 1.0f, 0.5f);
}

int main(int argc, char **argv)
{
	WHBProcInit();
	WHBGfxInit();
	romfsInit();
	setup_scene();

	while(WHBProcIsRunning())
	{
		WHBGfxBeginRender();

		// Render DRC
		WHBGfxBeginRenderDRC();
		WHBGfxClearColor(0.25f, 0.0f, 0.5f, 1.0f);
		prepare_render();
		render();
		WHBGfxFinishRenderDRC();

		// Render TV
		WHBGfxBeginRenderTV();
		WHBGfxClearColor(0.25f, 0.0f, 0.5f, 1.0f);
		prepare_render();
		render();
		WHBGfxFinishRenderTV();

		WHBGfxFinishRender();
	}

	WHBGfxShutdown();
	takedown_scene();
	romfsExit();
	WHBProcShutdown();
	
	return 0;
}
