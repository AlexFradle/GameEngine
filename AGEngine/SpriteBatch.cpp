#include "SpriteBatch.h"
#include <algorithm>

namespace AGEngine {
	SpriteBatch::SpriteBatch() : _vbo(0), _vao(0) {

	}

	SpriteBatch::~SpriteBatch() {

	}

	void SpriteBatch::init() {
		createVertexArray();
	}

	void SpriteBatch::begin(GlyphSortType sortType /*GlyphSortType::TEXTURE*/) {
		_sortType = sortType;
		_renderBatches.clear();
		for (auto g : _glyphs) {
			delete g;
		}
		_glyphs.clear();
	}

	void SpriteBatch::end() {
		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4 &destRect, const glm::vec4 &uvRect, GLuint texture, float depth, const Colour &colour) {
		// destRect.x = bottom left x, destRect.y = bottom left y, destRect.z = width, destRect.w = height
		Glyph *newGlyph = new Glyph;
		newGlyph->texture = texture;
		newGlyph->depth = depth;

		newGlyph->topLeft.colour = colour;
		newGlyph->topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		newGlyph->topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		newGlyph->bottomLeft.colour = colour;
		newGlyph->bottomLeft.setPosition(destRect.x, destRect.y);
		newGlyph->bottomLeft.setUV(uvRect.x, uvRect.y);

		newGlyph->bottomRight.colour = colour;
		newGlyph->bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		newGlyph->bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		newGlyph->topRight.colour = colour;
		newGlyph->topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		newGlyph->topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		_glyphs.push_back(newGlyph);
	}

	void SpriteBatch::renderBatch() {
		glBindVertexArray(_vao);
		for (auto &rb : _renderBatches) {
			glBindTexture(GL_TEXTURE_2D, rb.texture);
			glDrawArrays(GL_TRIANGLES, rb.offset, rb.numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		std::vector<Vertex> vertices;
		vertices.resize(_glyphs.size() * 6);  // 6 vertices per glyph

		if (_glyphs.empty())
			return;

		int offset = 0;
		int curVertex = 0;
		_renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
		vertices[curVertex++] = _glyphs[0]->topLeft;
		vertices[curVertex++] = _glyphs[0]->bottomLeft;
		vertices[curVertex++] = _glyphs[0]->bottomRight;
		vertices[curVertex++] = _glyphs[0]->bottomRight;
		vertices[curVertex++] = _glyphs[0]->topRight;
		vertices[curVertex++] = _glyphs[0]->topLeft;
		offset += 6;

		for (int curGlyph = 1; curGlyph < _glyphs.size(); curGlyph++) {
			if (_glyphs[curGlyph]->texture != _glyphs[curGlyph - 1]->texture)
				_renderBatches.emplace_back(offset, 6, _glyphs[curGlyph]->texture);
			else
				_renderBatches.back().numVertices += 6;
			vertices[curVertex++] = _glyphs[curGlyph]->topLeft;
			vertices[curVertex++] = _glyphs[curGlyph]->bottomLeft;
			vertices[curVertex++] = _glyphs[curGlyph]->bottomRight;
			vertices[curVertex++] = _glyphs[curGlyph]->bottomRight;
			vertices[curVertex++] = _glyphs[curGlyph]->topRight;
			vertices[curVertex++] = _glyphs[curGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		// orphan the data
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr , GL_DYNAMIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void SpriteBatch::createVertexArray() {
		// vao stores all of these states so you dont have to set them every draw call
		if (_vao == 0)
			glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);

		if(_vbo == 0)
			glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		// Position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));

		// Colour attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void *)offsetof(Vertex, colour));

		// UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs() {
		switch (_sortType) {
		case GlyphSortType::BACK_TO_FRONT:
			std::stable_sort(
				_glyphs.begin(),
				_glyphs.end(),
				[](Glyph *a, Glyph *b) {return (a->depth < b->depth); }
			);
			break;
		case GlyphSortType::FRONT_TO_BACK:
			std::stable_sort(
				_glyphs.begin(), 
				_glyphs.end(),
				[](Glyph *a, Glyph *b) {return (a->depth > b->depth); }
			);
			break;
		case GlyphSortType::TEXTURE:
			std::stable_sort(
				_glyphs.begin(), 
				_glyphs.end(), 
				[](Glyph *a, Glyph *b) {return (a->texture < b->texture); }
			);
			break;
		}
	}
}
