DoOnce 'info.lua'

local BuildFlags
if tup.getconfig('PLATFORM') == 'ubuntu12' or
	tup.getconfig('PLATFORM') == 'ubuntu12_64' or
	tup.getconfig('PLATFORM') == 'arch64'
then
	BuildFlags = '-DRENGENERAL_FILESYSTEM_DATALOCATION=\\"/usr/share/' .. Info.PackageName .. '\\"'
end

GeneralObjects = Define.Objects
{ 
	Sources = Item '*.cxx',
	BuildFlags = BuildFlags
}

