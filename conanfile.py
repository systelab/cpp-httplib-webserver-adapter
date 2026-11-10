import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain
from conan.tools.files import collect_libs, copy

required_conan_version = ">=2.0.0"

class HttpLibWebServerAdapterConan(ConanFile):
    name = "httplib-web-server-adapter"
    description = "Implementation of C++ Web Server based on cpp-httplib"
    url = "https://github.com/systelab/cpp-httplib-webserver-adapter"
    homepage = "https://github.com/systelab/cpp-httplib-webserver-adapter"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "web", "server", "http", "ssl", "httplib", "adapter")
    license = "MIT"
    package_type = "static-library"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*", "!build*", "!*.yml", "!*.xml", "!*.md", "!*.in", "!ci", "!.gitattributes", "!.gitignore"

    def configure(self):
        self.options["openssl/*"].shared = True

    def requirements(self):
        self.requires("web-server-adapter-interface/2.0.3", transitive_headers=True)
        self.requires("openssl/3.0.19")
        self.requires("zlib/1.3.1")

    def build_requirements(self):
        self.test_requires("gtest/1.14.0")
        self.test_requires("web-server-adapter-test-utilities/2.0.3")
        self.test_requires("test-utilities-interface/1.1.0")

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

    def package(self):
        header_src = os.path.join(self.source_folder, "src", "HttpLibWebServerAdapter")
        header_dst = os.path.join(self.package_folder, "include", "HttpLibWebServerAdapter")
        copy(self, "ClientFactory.h", src=header_src, dst=header_dst)
        copy(self, "ServerFactory.h", src=header_src, dst=header_dst)
        lib_dst = os.path.join(self.package_folder, "lib")
        copy(self, "*HttpLibWebServerAdapter.lib", src=self.build_folder, dst=lib_dst, keep_path=False)
        copy(self, "*HttpLibWebServerAdapter.pdb", src=self.build_folder, dst=lib_dst, keep_path=False)
        copy(self, "*HttpLibWebServerAdapter.a", src=self.build_folder, dst=lib_dst, keep_path=False)

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "HttpLibWebServerAdapter")
        self.cpp_info.set_property("cmake_target_name", "HttpLibWebServerAdapter::HttpLibWebServerAdapter")
        self.cpp_info.libs = collect_libs(self)
