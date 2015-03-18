#ifndef SIMPLE_GRAPHICS_ENGINE_H
#define SIMPLE_GRAPHICS_ENGINE_H

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl/glew.h>
#include <gl/glfw3.h>

class Material {
public:
  Material(GLuint program_ID);
  void render();
  
  glm::vec3 diffuse_color_;
  glm::vec3 specular_color_;
  float specularity_;
  int shinyness_;
  
private:
  GLuint program_ID_;
  
  GLuint diffuseColor_ID_;
  GLuint specularColor_ID_;
  GLuint specularity_ID_;
  GLuint shinyness_ID_;
};

class Transform {
public:
  Transform();
  
  glm::vec3 getPosition(){return position_;};
  glm::vec3 getScale(){return scale_;};
  glm::mat4 getMatrix(){return matrix_;};
  glm::vec3 getEulerRotationXYZ(){return rotation_;};
  
  void translate(glm::vec3 position);
  void scale(glm::vec3 scale);
  //void rotate(float angle, glm::vec3 rotation_axis);
  void rotateX(float angle);
  void rotateY(float angle);
  void rotateZ(float angle);
  void reset();
private:
  glm::vec3 position_;
  glm::vec3 scale_;
  glm::vec3 rotation_; // angleX, angleY, angleZ
  glm::mat4 matrix_;
};

class Object3D {
public:
  Object3D() {};
  virtual ~Object3D() {};
  void addChild(Object3D* child);
  virtual void render(glm::mat4 M);
  // Todo update function here

  Transform transform_;
private:
  std::vector<Object3D*> children;
};

class BoundingBox;

class AbstractMesh : public Object3D{
public:
  AbstractMesh();
  ~AbstractMesh();
  
  void normalizeScale();
  
  virtual void render(glm::mat4 M) = 0;
protected:
  virtual void initialize() = 0;
  
  GLuint program_ID_;
  std::vector<glm::vec3> vertices_;
  std::vector<unsigned short> elements_; // Maximum around 60000 vertices for unsigned short.
  GLuint vertex_array_ID_;
  GLuint vertex_buffer_;
  GLuint element_buffer_;
  
  GLuint model_matrix_ID_;
private:
  friend BoundingBox;
};


class TriangleMesh : public AbstractMesh{
public:
  TriangleMesh(const char *file_name, GLuint program_ID);
  TriangleMesh(std::vector<glm::vec3> vertices,
       std::vector<glm::vec3> normals,
       std::vector<unsigned short> elements,
       GLuint program_ID);
  TriangleMesh(GLuint program_ID);
  ~TriangleMesh();
  
  void initPlane(glm::vec3 position, glm::vec3 normal, glm::vec3 scale);
  void initBox(glm::vec3 max, glm::vec3 min, glm::vec3 position);
  
  virtual void render(glm::mat4 M);
  Material material_;
private:
  friend BoundingBox;

  void initialize();

  GLuint normal_buffer_;
  
  std::vector<glm::vec3> normals_;
};

class LineMesh : public AbstractMesh {
public:
  LineMesh(GLuint program_ID);
  ~LineMesh();
  
  void initAxes();
  void initGridPlane(
                     glm::vec3 position,
                     glm::vec3 normal,
                     glm::vec3 scale,
                     unsigned int divisions);
  
  virtual void render(glm::mat4 M);
private:
  void initialize();
};

class BoundingBox{
public:
  BoundingBox(const AbstractMesh* template_mesh);
  
  glm::vec3 getMin(){return min;}
  glm::vec3 getMax(){return max;}
  
private:
  glm::vec3 max;
  glm::vec3 min;
};

class Camera : public Object3D {
public:
  Camera(GLuint program_ID, GLFWwindow* window_);
  void render(glm::mat4 M);
  
  glm::mat4 getViewProjectionMatrix();
private:
  GLuint program_ID_;

  GLuint view_matrix_ID_;
  GLuint projection_matrix_ID_;
  
  glm::mat4 view_transform_;
  glm::mat4 projection_transform_;
  
  GLFWwindow* window_;
};

class LightSource : public Object3D {
public:
  LightSource(GLuint program_ID);
  virtual void render(glm::mat4 M);
  
  float intensity_;
  glm::vec3 color_;
private:
  GLuint program_ID_;
  
  GLuint light_position_ID_;
  GLuint light_intensity_ID_;
  GLuint light_color_ID_;
};

class SimpleGraphicsEngine {
public:
  SimpleGraphicsEngine();
  virtual ~SimpleGraphicsEngine();
  void run();
protected:
  virtual void update() = 0;
  double dt_;
  Object3D* scene_;
  
  static Object3D* camera_;

  GLuint program_ID_basic_render_;
  GLuint program_ID_axis_shader_;
  GLuint program_ID_grid_plane_shader_;
  
  // Probably should be private...
  GLFWwindow* window_;
private:
  bool initialize();

  LineMesh* axes_;
  LineMesh* grid_plane_;
  
  // One camera for each shader
  Camera* basic_cam_;
  Camera* axis_cam_;
  Camera* grid_plane_cam_;
  
  double time_;
};

#endif