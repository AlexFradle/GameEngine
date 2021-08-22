#pragma once
#include <GL/glew.h>

namespace AGEngine {
	struct Position {
		float x;
		float y;
	};

	struct Colour {
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;

		void setColour(GLubyte newR, GLubyte newG, GLubyte newB, GLubyte newA) {
			r = newR;
			g = newG;
			b = newB;
			a = newA;
		}
	};

	struct UV {
		float u;
		float v;
	};

	struct Vertex {
		Position position;
		Colour colour;
		UV uv;

		void setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
			colour.setColour(r, g, b, a);
		}

		void setUV(float u, float v) {
			uv.u = u;
			uv.v = v;
		}
	};
}
