lib_name = "clipboard"
dependencies = {  }

solution (lib_name)
	configurations { "Debug", "Release" }

	project (lib_name)
		kind "StaticLib"
		language "C"
		location "build"
		targetdir "build/lib"
		includedirs { "include" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
 
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "linux"
			files { "src/linux.c" }

		configuration "windows"
			files { "src/windows.c" }

	ex_dependencies = {"allegro","allegro_image" }
	examples = os.matchfiles("examples/*.c")
	for index, name in pairs(examples) do
		sname = name:sub(10, name:len()-2);
		project (sname)
			kind "ConsoleApp"
			language "C"
			location "build"
			files { name }
			includedirs { "src" }
--			libdirs { "../lib" }
			links (lib_name)
			links (ex_dependencies)
			targetdir "build/examples"
--			postbuildcommands { "cd .. && build/examples/"..sname }

			configuration "Debug"
				defines { "DEBUG" }
				flags { "Symbols", "ExtraWarnings" }
	 
			configuration "Release"
				defines { "NDEBUG" }
				flags { "Optimize", "ExtraWarnings" }
	end

newoption {
   trigger     = "dir",
   value       = "path",
   description = "Choose a path to install dir",
}

newaction {
	trigger     = "install",
	description = "Install the software",
	execute = function ()
		-- copy files, etc. here
		os.mkdir(_OPTIONS["dir"].."lib/");
		files = os.matchfiles("build/lib/*")
		print ("Installing lib files to " .. _OPTIONS["dir"] .."lib/")
		for k, f in pairs(files) do
			print ("Copying " .. f)
			os.copyfile(f, _OPTIONS["dir"].."lib/")
		end
		os.mkdir(_OPTIONS["dir"].."include/"..lib_name.."/");
		files = os.matchfiles("src/*.h")
		print ("Installing header files to " .. _OPTIONS["dir"] .."include/")
		for k, f in pairs(files) do
			print ("Copying " .. f)
			os.copyfile(f, _OPTIONS["dir"].."include/"..lib_name.."/")
		end
	end
}

if not _OPTIONS["dir"] then
   _OPTIONS["dir"] = "/usr/local/"
end

if not ("/" == _OPTIONS["dir"]:sub(_OPTIONS["dir"]:len())) then
	_OPTIONS["dir"] = _OPTIONS["dir"] .. "/"
end
