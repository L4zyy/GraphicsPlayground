#pragma once

#include "Global.h"

namespace gpg {

	struct Vertex {
		Eigen::Vector3d pos;
		Eigen::Vector3d rgb;
		double x;
		double y;
		double z;
		double r;
		double g;
		double b;

		Vertex()
			: pos(), rgb(), x(), y(), z(), r(), g(), b() {}
		Vertex(double _x, double _y, double _z, double _r, double _g, double _b)
			: pos(_x, _y, _z), rgb(_r, _g, _b), x(_x), y(_y), z(_z), r(_r), g(_g), b(_b) {}
		Vertex(Eigen::Vector3d _pos, Eigen::Vector3d _rgb)
			: pos(_pos), rgb(_rgb), x(pos[0]), y(pos[1]), z(pos[2]), r(rgb[0]), g(rgb[1]), b(rgb[2]) {}
	};

	struct EdgeEquation {
		double a;
		double b;
		double c;
		bool tie;

		EdgeEquation(const Vertex& v0, const Vertex& v1) {
			a = v0.y - v1.y;
			b = v1.x - v0.x;
			c = -(a * (v0.x + v1.x) + b * (v0.y + v1.y)) / 2;
			tie = a != 0 ? a > 0 : b > 0;
		}

		double evaluate(double x, double y) {
			return a * x + b * y + c;
		}

		bool test(double x, double y) {
			return test(evaluate(x, y));
		}

		bool test(double v) {
			return (v > 0 || v == 0 && tie);
		}
	};

	struct ParameterEquation {
		double a;
		double b;
		double c;

		ParameterEquation(
			double p0,
			double p1,
			double p2,
			const EdgeEquation& e0,
			const EdgeEquation& e1,
			const EdgeEquation& e2,
			double area
		) {
			double factor = 1.0f / (2.0f * area);

			a = factor * (p0 * e1.a + p1 * e2.a + p2 * e0.a);
			b = factor * (p0 * e1.b + p1 * e2.b + p2 * e0.b);
			c = factor * (p0 * e1.c + p1 * e2.c + p2 * e0.c);
		}

		double evaluate(double x, double y) {
			return a * x + b * y + c;
		}
	};

}
