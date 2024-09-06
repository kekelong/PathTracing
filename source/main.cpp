#include "camera/camera.hpp"
#include "shape/sphere.hpp"
#include "shape/model.hpp"
#include "shape/plane.hpp"
#include "shape/scene.hpp"
#include "util/rgb.hpp"
#include "material/diffuse_material.hpp"
#include "material/specular_material.hpp"
#include "material/dielectric_material.hpp"
#include "material/conductor_material.hpp"
#include "material/ground_material.hpp"
#include "renderer/normal_renderer.hpp"
#include "renderer/path_tracing_renderer.hpp"
#include "renderer/debug_renderer.hpp"

#ifdef PROJECT_ROOT_DIR
	std::string root_dir = PROJECT_ROOT_DIR;
	std::string asset_dir = root_dir + "/asset/dragon_871k.obj";
	std::string resource_dir = root_dir + "/resource";
#else
	# error "PROJECT_ROOT_DIR is not defined!"
	/**	×Ô¶¨Òå
	 * 	std::string root_dir = PROJECT_ROOT_DIR;
	 * 	std::string asset_dir = root_dir + "/asset/dragon_871k.obj";
	 * 	std::string resource_dir = root_dir + "/resource";
	 */
#endif

int main() {
	Film film{ 192 * 4, 108 * 4 };
	Camera camera{ film, { -10, 1.5, 0 }, { 0, 0, 0 }, 45 };

	Model model(asset_dir);
	Sphere sphere{
		{ 0, 0, 0 },
		1
	};
	Plane plane{
		{ 0, 0, 0 },
		{ 0, 1, 0 }
	};

	Scene scene{};
	for (int i = -3; i <= 3; i++) {
		scene.addShape(
			sphere,
			new DielectricMaterial{ 1.f + 0.2f * (i + 3), { 1, 1, 1 } },
			{ 0, 0.5, i * 2 },
			{ 0.8, 0.8, 0.8 }
		);
	}
	for (int i = -3; i <= 3; i++) {
		glm::vec3 c = RGB::GenerateHeatmapRGB((i + 3.f) / 6.f);
		scene.addShape(
			sphere,
			new ConductorMaterial{
				glm::vec3(2.f - c * 2.f),
				glm::vec3(2.f + c * 3.f),
			},
			{ 0, 2.5, i * 2 },
			{ 0.8, 0.8, 0.8 }
		);
	}
	scene.addShape(
		model,
		new DielectricMaterial{ 1.8, RGB(128, 191, 131) },
		{ -5, 0.4, 1.5 },
		{ 2, 2, 2 }
	);
	scene.addShape(
		model,
		new ConductorMaterial{ { 0.1, 1.2, 1.8 }, { 5, 2.5, 2 } },
		{ -5, 0.4, -1.5 },
		{ 2, 2, 2 }
	);
	scene.addShape(plane, new GroundMaterial{ RGB(120, 204, 157) }, { 0, -0.5, 0 });
	auto* light_material = new DiffuseMaterial{ { 1, 1, 1 } };
	light_material->setEmissive({ 0.95, 0.95, 1 });
	scene.addShape(plane, light_material, { 0, 10, 0 });
	scene.build();

	//NormalRenderer normal_renderer{ camera, scene };
	//normal_renderer.render(1, "normal.ppm");

	//BoundsTestCountRenderer btc_renderer{ camera, scene };
	//btc_renderer.render(1, "BTC.ppm");
	//TriangleTestCountRenderer ttc_renderer{ camera, scene };
	//ttc_renderer.render(1, "TTC.ppm");

	PathTracingRenderer path_tracing_renderer{ camera, scene };
	path_tracing_renderer.render(512, resource_dir+"/PT_cosine_test.ppm");

	return 0;
}