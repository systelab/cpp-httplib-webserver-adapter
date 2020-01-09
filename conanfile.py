from conans import ConanFile, tools

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
    options = {"gtest": ["1.7.0", "1.8.1"], "OpenSSL": ["1.0.2n"]}
    default_options = {"gtest":"1.8.1", "OpenSSL":"1.0.2n"}

    def configure(self):
        self.options["WebServerAdapterTestUtilities"].gtest = self.options.gtest
        self.options["OpenSSL"].shared = True
        self.options["boost"].shared = True

    def requirements(self):
        self.requires("WebServerAdapterInterface/1.0.2@systelab/stable")
        self.requires(("OpenSSL/%s@conan/stable") % self.options.OpenSSL)

    def build_requirements(self):
        self.build_requires("WebServerAdapterTestUtilities/1.0.2@systelab/stable")
        if self.options.gtest == "1.7.0":
            self.build_requires("gtest/1.7.0@systelab/stable")
        else:
            self.build_requires("gtest/1.8.1@bincrafters/stable")

    def imports(self):
        self.copy("*.dll", dst=("bin/%s" % self.settings.build_type), src="bin")
        self.copy("*.dylib*", dst=("bin/%s" % self.settings.build_type), src="lib")
        self.copy("*.so*", dst=("bin/%s" % self.settings.build_type), src="lib")

    def package(self):
        self.copy("ServerFactory.h", dst="include/HttpLibWebServerAdapter", src="src/HttpLibWebServerAdapter")
        self.copy("*HttpLibWebServerAdapter.lib", dst="lib", keep_path=False)
        self.copy("*HttpLibWebServerAdapter.pdb", dst="lib", keep_path=False)
        self.copy("*HttpLibWebServerAdapter.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = tools.collect_libs(self)
