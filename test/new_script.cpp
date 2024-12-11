#include "api.hpp"
#include <stdexcept>

Variant measure_obj_callp(Node obj) {
	//return obj("get_name");
	return obj.get_name();
}

static inline int lookup_function(std::vector<Vector3> &vertices, const Vector3& dv) {
	for (int i = vertices.size()-1; i >= 0; i--) {
		if (vertices[i].distance_squared_to(dv) < 0.001f)
			return i;
	}
	const int index = vertices.size();
	vertices.push_back(dv);
	return index;
}

Variant createSkyDome(double r_inner64, double r_outer64)
{
	const float r_inner = r_inner64;
	const float r_outer = r_outer64;
	static constexpr int numX = 16;
	static constexpr int numY = 4;
	static constexpr int total_points = (2 * numY + 1) * numX * 3; // triangles * 3
	static constexpr float PI = M_PI;

	std::vector<Vector3> vertices; vertices.reserve(numX * numY + 1);
	std::array<int, total_points> indices;
	size_t index_count = 0;

	float stepa  = PI * 2.0f / numX;
	float startb = Math::asinf(r_inner / r_outer);
	float stepb = (PI / 2.0f - startb) / 4.0f;

	for (int y = 0; y < numY; y++)
	{
		float b = startb + stepb * y;

		for (int x = 0; x < numX; x++)
		{
			float a = stepa * x;

			Vector3 va( Math::sinf(a)         * Math::cosf(b) * r_outer,         Math::sinf(b) * r_outer,         -Math::cosf(a)         * Math::cosf(b) * r_outer );
			Vector3 vb( Math::sinf(a + stepa) * Math::cosf(b) * r_outer,         Math::sinf(b) * r_outer,         -Math::cosf(a + stepa) * Math::cosf(b) * r_outer );
			Vector3 vc( Math::sinf(a + stepa) * Math::cosf(b + stepb) * r_outer, Math::sinf(b + stepb) * r_outer, -Math::cosf(a + stepa) * Math::cosf(b + stepb) * r_outer );
			Vector3 vd( Math::sinf(a)         * Math::cosf(b + stepb) * r_outer, Math::sinf(b + stepb) * r_outer, -Math::cosf(a)         * Math::cosf(b + stepb) * r_outer );

			// Find va, vb, vc, vd in the lookup hashmap and add their index to the indices vector
			// Triangle 1: va, vb, vc
			indices[index_count++] = lookup_function(vertices, va);
			const int va_idx = indices.back();
			indices[index_count++] = lookup_function(vertices, vb);
			indices[index_count++] = lookup_function(vertices, vc);
			const int vc_idx = indices.back();
			// Triangle 2: vc, vd, va
			indices[index_count++] = vc_idx;
			indices[index_count++] = lookup_function(vertices, vd);
			indices[index_count++] = va_idx;
		}
	}

	float b = startb + stepb * 3.0f;

	for (int x = 0; x < numX; x++)
	{
		float a = stepa * x;

		Vector3 va( Math::sinf(a)         * Math::cosf(b) * r_outer, Math::sinf(b) * r_outer, -Math::cosf(a)         * Math::cosf(b) * r_outer );
		Vector3 vb( Math::sinf(a + stepa) * Math::cosf(b) * r_outer, Math::sinf(b) * r_outer, -Math::cosf(a + stepa) * Math::cosf(b) * r_outer );
		Vector3 vc( 0.0f, r_outer, 0.0f );

		indices[index_count++] = lookup_function(vertices, va);
		indices[index_count++] = lookup_function(vertices, vb);
		indices[index_count++] = lookup_function(vertices, vc);
	}

	if (index_count != total_points) {
		fprintf(stderr, "Error: Skydome index mismatch: %zu != %d", indices.size(), total_points);
		throw std::runtime_error("Skydome index mismatch");
	}
	//print(vertices.size());

	return Nil;
}
