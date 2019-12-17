// Copyright © Matt Jones and Contributors. Licensed under the MIT Licence (MIT). See LICENCE.md in the repository root for more information.

#include <iostream>
#include "NovelRenderObject.h"
#include "GeoBounds.h"
#include <functional>

namespace NovelRT {

  void NovelRenderObject::OnCameraViewChanged(CameraViewChangedEventArgs args) {
    _uboCameraData = CameraBlock(args.cameraMatrix.getUnderlyingMatrix());
  }

  NovelRenderObject::NovelRenderObject(NovelLayeringService* layeringService,
    const GeoVector<float>& size,
    const NovelCommonArgs& args,
    ShaderProgram shaderProgram,
    NovelCamera* camera) : NovelWorldObject(layeringService, size, args),
                           _finalViewMatrixData(Lazy<CameraBlock>(std::function<CameraBlock()>(std::bind(&NovelRenderObject::generateViewData, this)))),
                           _vertexBuffer(Lazy<GLuint>(std::function<GLuint()>(generateStandardBuffer))),
                           _vertexArrayObject(Lazy<GLuint>(std::function<GLuint()>([] {
                                                                                        GLuint tempVao;
                                                                                        glGenVertexArrays(1, &tempVao);
                                                                                        return tempVao;
                           }))),
                           _shaderProgram(shaderProgram),
                           _camera(camera),
                           _uboCameraData(CameraBlock(_camera->getCameraUboMatrix().getUnderlyingMatrix())) {
    _camera->subscribeToCameraViewChanged([this](auto x) { OnCameraViewChanged(x); }); //TODO: Should we refactor the method into the lambda?
  }

      void NovelRenderObject::executeObjectBehaviour() {
        if (!_bufferInitialised) {
          configureObjectBuffers();
          _bufferInitialised = true;
        }
        drawObject();
      }

      void NovelRenderObject::configureObjectBuffers() {
        auto topLeft = GeoVector<GLfloat>(-1.0f, 1.0f);
        auto bottomRight = GeoVector<GLfloat>(1.0f, -1.0f);
        auto topRight = GeoVector<GLfloat>(1.0f, 1.0f);
        auto bottomLeft = GeoVector<GLfloat>(-1.0f, -1.0f);

        _vertexBufferData = {
            topLeft.getX(), topLeft.getY(), 0.0f,
            bottomRight.getX(), bottomRight.getY(), 0.0f,
            topRight.getX(), topRight.getY(), 0.0f,
            topLeft.getX(), topLeft.getY(), 0.0f,
            bottomLeft.getX(), bottomLeft.getY(), 0.0f,
            bottomRight.getX(), bottomRight.getY(), 0.0f,
        };

        _vertexArrayObject.getActual(); //this is just here to force initialisation.

      // The following commands will talk about our 'vertexbuffer' buffer
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer.getActual());

        // Give our vertices to OpenGL.
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * _vertexBufferData.size(), _vertexBufferData.data(), GL_STATIC_DRAW);
      }

      void NovelRenderObject::setSize(const GeoVector<float>& value) {
        _finalViewMatrixData.reset();
        NovelWorldObject::setSize(value);
        configureObjectBuffers();
      }
      void NovelRenderObject::setRotation(const float value) {
        _finalViewMatrixData.reset();
        NovelWorldObject::setRotation(value);
        configureObjectBuffers();
      }
      void NovelRenderObject::setScale(const GeoVector<float>& value) {
        _finalViewMatrixData.reset();
        NovelWorldObject::setScale(value);
        configureObjectBuffers();
      }
      void NovelRenderObject::setPosition(const GeoVector<float>& value) {
        _finalViewMatrixData.reset();
        NovelWorldObject::setPosition(value);
        configureObjectBuffers();
      }
      NovelRenderObject::~NovelRenderObject() {
        if (!_vertexArrayObject.isCreated())
          return;

        auto vao = _vertexArrayObject.getActual();
        glDeleteVertexArrays(1, &vao);
      }

      GLuint NovelRenderObject::generateStandardBuffer() {
        GLuint tempBuffer;
        glGenBuffers(1, &tempBuffer);
        return tempBuffer;
      }

      CameraBlock NovelRenderObject::generateViewData() {
        auto size = (getSize() * getScale()).getVec2Value();
        auto position = getPosition().getVec2Value();
        auto matrix3D = glm::mat4();
        glm::rotate(matrix3D, glm::radians(getRotation()), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::scale(matrix3D, glm::vec3(size, 1.0f));
        glm::translate(matrix3D, glm::vec3(position, 0.0f));
        return CameraBlock(matrix3D * _uboCameraData.cameraMatrix);
      }

      CameraBlock NovelRenderObject::generateCameraBlock() {
        return CameraBlock(_camera->getCameraUboMatrix().getUnderlyingMatrix());
      }
}
