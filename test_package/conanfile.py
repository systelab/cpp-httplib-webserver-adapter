import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy

class HttpLibWebServerAdapterTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    def layout(self):
        cmake_layout(self)

    def configure(self):
        self.options["openssl/*"].shared = True

    def requirements(self):
        self.requires(self.tested_reference_str)

    def generate(self):
        CMakeDeps(self).generate()
        CMakeToolchain(self).generate()

        runtime_dst = os.path.join(self.build_folder, "bin", str(self.settings.build_type))
        for dep in self.dependencies.values():
            for bindir in dep.cpp_info.bindirs:
                copy(self, "*.dll", src=bindir, dst=runtime_dst)
            for libdir in dep.cpp_info.libdirs:
                copy(self, "*.dylib*", src=libdir, dst=runtime_dst)
                copy(self, "*.so*", src=libdir, dst=runtime_dst)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def test(self):
        cmake = CMake(self)
        cmake.test()
