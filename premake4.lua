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

--	ex_dependencies = {"allegro","allegro_image" }
	examples = os.matchfiles("examples/*.c")
	for index, name in pairs(examples) do
		sname = name:sub(10, name:len()-2);
		project (sname)
			kind "ConsoleApp"
			language "C"
			location "build"
			files { name }
			includedirs { "src" }
			links (lib_name)
--			links (ex_dependencies)
			targetdir "build/examples"

			configuration "Debug"
				defines { "DEBUG" }
				flags { "Symbols", "ExtraWarnings" }
--            links {"alld"}
			configuration "Release"
				defines { "NDEBUG" }
				flags { "Optimize", "ExtraWarnings" }
--				links {"alleg"}
	end

slash = "/"
if os.is("windows") then
   slash = "\\"
end

newoption {
   trigger     = "dir",
   value       = "path",
   description = "Choose a path to install dir",
}

-- note : os.matchfiles expects directory separators to be forward slashes
newaction {
	trigger     = "install",
	description = "Install the software",
	execute = function ()
		-- copy files, etc. here

		libdir = _OPTIONS["dir"].."lib".."/"
		os.mkdir(libdir);
		files = os.matchfiles("build/lib/*")
		print ("")
		print ("Installing lib files to " .. libdir)
		for k, f in pairs(files) do
			print ("Copying " .. f)
			success , errormsg = os.copyfile(f, libdir..getshortfilename(f))
			if (success == nil) then
            print(errormsg)
         end
		end

		includedir = _OPTIONS["dir"].."include/"..lib_name.."/"
		os.mkdir(includedir);
		files = os.matchfiles("src/*.h")
		print ("")
		print ("Installing header files to " .. includedir)
		for k, f in pairs(files) do
			print ("Copying " .. f)
			success , errormsg = os.copyfile(f , includedir..getshortfilename(f))
			if (success == nil) then
            print(errormsg)
         end
		end

	end
}

newaction {
   trigger = "uninstall",
   description = "Uninstall the software",
   execute = function ()

		libdir = _OPTIONS["dir"].."lib".."/"
		files = os.matchfiles("build/lib/*")
		print ("")
		print ("Uninstalling lib files from " .. libdir)
		for k, f in pairs(files) do
			print ("Removing " .. f)
			success , errormsg = os.remove(libdir..getshortfilename(f))
			if (success == nil) then
            print(errormsg)
         end
		end

		includedir = _OPTIONS["dir"].."include/"..lib_name.."/"
		files = os.matchfiles("src/*.h")
		print ("")
		print ("Uninstalling header files from " .. includedir)
		for k, f in pairs(files) do
			print ("Removing " .. f)
			success , errormsg = os.remove(includedir..getshortfilename(f))
			if (success == nil) then
            print(errormsg)
         end
		end
		print ("Removing "..includedir)
		success , errormsg = os.rmdir(includedir)
		if (success == nil) then
		   print(errormsg)
      end
		
   end
}

if not _OPTIONS["dir"] then
   _OPTIONS["dir"] = "/usr/local/"
end

_OPTIONS["dir"] = string.gsub(_OPTIONS["dir"] , "\\" , "/")

if not ("/" == _OPTIONS["dir"]:sub(_OPTIONS["dir"]:len())) then
	_OPTIONS["dir"] = _OPTIONS["dir"] .. "/"
end


function getshortfilename(fullfilename)
   -- first, replace all backslashes with forward slashes
   if (type(fullfilename) ~= "string") then
      return nil 
   end
   shorter = string.gsub(fullfilename , "\\" , "/")
   while (true) do
      start , stop = string.find(shorter , "/")
      if (start == nil) then break end
      shorter = string.sub(shorter , stop + 1)
   end
   return shorter
end



