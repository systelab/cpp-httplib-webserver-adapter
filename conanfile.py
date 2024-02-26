from conans import ConanFile, tools, CMake

class HttpLibWebServerAdapterConan(ConanFile):
    name = "HttpLibWebServerAdapter"
    description = "Implementation of C++ Web Server based on cpp-httplib"
    url = "https://github.com/systelab/cpp-httplib-webserver-adapter"
    homepage = "https://github.com/systelab/cpp-httplib-webserver-adapter"
    author = "CSW <csw@werfen.com>"
    topics = ("conan", "web", "server", "http", "ssl", "httplib", "adapter")
    license = "MIT"
    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "*", "!build*", "!*.yml", "!*.xml", "!*.md", "!*.in", "!ci", "!.gitattributes", "!.gitignore"

    def configure(self):
        self.options["openssl"].shared = True

    def requirements(self):
        self.requires("WebServerAdapterInterface/2.0.2@systelab/stable")
        self.requires("zlib/1.2.13#13c96f538b52e1600c40b88994de240f")
        self.requires("openssl/3.0.12#1670458f93ec138c3bb6afc65a1cd667")
        self.requires("gtest/1.14.0#4372c5aed2b4018ed9f9da3e218d18b3", private=True)
        self.requires("WebServerAdapterTestUtilities/2.0.2@systelab/stable", private=True)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dylib*", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.so*", dst=("bin/%s" % self.settings.build_type), src="lib")

    def package(self):
        self.copy("ClientFactory.h", dst="include/HttpLibWebServerAdapter", src="src/HttpLibWebServerAdapter")
        self.copy("ServerFactory.h", dst="include/HttpLibWebServerAdapter", src="src/HttpLibWebServerAdapter")
        self.copy("*HttpLibWebServerAdapter.lib", dst="lib", keep_path=False)
        self.copy("*HttpLibWebServerAdapter.pdb", dst="lib", keep_path=False)
        self.copy("*HttpLibWebServerAdapter.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
