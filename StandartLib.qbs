import qbs

StaticLibrary {
	Depends { name: "cpp" }
	cpp.cppFlags: "-std=c++11"
	Export {
		Depends { name: "cpp" }
		cpp.cppFlags: "-std=c++11"
		cpp.includePaths: product.sourceDirectory
		cpp.defines: ["USING_" + product.name.toUpperCase()]
	}
	Group {
		name: 'src'
		files: ["*.c", "*.h", "*.cpp"]
		prefix: "**/"
	}
}
