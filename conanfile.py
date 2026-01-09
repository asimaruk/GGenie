from conan import ConanFile
from conan.tools.cmake import cmake_layout


class GgenieRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("opengl/system")
        self.requires("glfw/3.4")
        self.requires("glm/1.0.1")
        self.requires("freetype/2.13.3")

    def build_requirements(self):
        self.tool_requires("cmake/4.2.1")
        self.test_requires("catch2/3.8.1")

    def layout(self):
        cmake_layout(self)