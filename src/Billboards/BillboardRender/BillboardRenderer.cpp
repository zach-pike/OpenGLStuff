#include "BillboardRenderer.hpp"

BillboardRenderer::BillboardRenderer() {

}

BillboardRenderer::~BillboardRenderer() {

}

void BillboardRenderer::draw(glm::mat4 viewMatrix, glm::mat4 viewProjection) {
    for(auto ptr : drawObjects) {
        ptr->draw(viewMatrix, viewProjection);
    }
}