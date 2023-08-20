#include "commons.h"
#include <fstream>
#include <vector>
#include <locale>
#include <filesystem>

namespace PluginFS {
    template <typename T>
    class StreamGuard
    {
    public:
        StreamGuard(T& stream) : stream_(stream) {}
        ~StreamGuard()
        {
            if (stream_.is_open())
            {
                stream_.close();
            }
        }

    private:
        T& stream_;
    };

    static void removeTailingSlash(wstr& path) {
		if (!path.empty() && path.back() == L'/')
		{
			path.pop_back();
		}
	}

    static vec<wstr> ReadFile(wstr path)
    {
        std::wfstream inputStream;
        inputStream.open(path, std::ios::in);

        if (!inputStream.good())
        {
            return {};
        }

        PluginFS::StreamGuard<std::wfstream> guard(inputStream);

        std::locale utf8_locale = std::locale("en_US.UTF-8");
        inputStream.imbue(utf8_locale);

        vec<wstr> lines;
        wstr line;
        while (std::getline(inputStream, line))
        {
            lines.push_back(line);
        }

        return lines;
    }

    static bool WriteFile(wstr path, wstr& content, bool enableAppendMode)
    {
        std::wfstream outputStream;
        if (enableAppendMode) {
            outputStream.open(path, std::ios::out | std::ios::app);
        }
        else {
            outputStream.open(path, std::ios::out);
        }

        if (!outputStream.good())
        {
            return false;
        }

        PluginFS::StreamGuard<std::wfstream> guard(outputStream);

        std::locale utf8_locale = std::locale("en_US.UTF-8");
        outputStream.imbue(utf8_locale);

        outputStream << content;
        if (outputStream.fail() || outputStream.bad())
        {
            return false;
        }

        return true;
    }

    static bool MkDir(wstr pluginRoot, wstr relativePath)
	{
        PluginFS::removeTailingSlash(relativePath);
        std::filesystem::path fullPath{ pluginRoot + L"\\" + relativePath};
        if (std::filesystem::exists(fullPath)) {
            return false;
        }
        if (std::filesystem::create_directories(fullPath)) {
            return true;
        }
        return false;
	}

    using FileStat = struct{
        wstr fileName;
        bool isFile;
        bool isDir;
        int size;
    };

    static PluginFS::FileStat Stat(wstr path)
	{
        PluginFS::removeTailingSlash(path);
        auto entry = std::filesystem::directory_entry(path);

        return PluginFS::FileStat{
            entry.is_regular_file() ? entry.path().filename().wstring() : entry.path().stem(),
            entry.is_regular_file(),
            entry.is_directory(),
            static_cast<int>(entry.file_size())
        }; 
	}

    static vec<PluginFS::FileStat> ReadDir(wstr path) {
        PluginFS::removeTailingSlash(path);

		vec<PluginFS::FileStat> fileStats;
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
            wstr enryPathWstr = entry.path().wstring();
            PluginFS::removeTailingSlash(enryPathWstr);
            std::filesystem::path entryPath{ enryPathWstr };

			fileStats.push_back(PluginFS::FileStat{
				entry.is_regular_file() ? entryPath.filename().wstring() : entryPath.stem(),
				entry.is_regular_file(),
				entry.is_directory(),
				static_cast<int>(entry.file_size())
				});
		}
		return fileStats;
    }

}

V8Value* native_ReadFile(const vec<V8Value*>& args)
{
    wstr destPath = config::pluginsDir() + L"\\" + args[0]->asString()->str;
    if (utils::isFile(destPath)) {
        vec<wstr> lines = PluginFS::ReadFile(destPath);
        wstr content;
        for (wstr line : lines)
        {
            content += line + L"\n";
        }
        if (!content.empty())
        {
            content.erase(content.size() - 1);
        }
        CefStr content_cef_string{ content };
        return V8Value::string(&content_cef_string);
    }
    return V8Value::undefined();
}

V8Value* native_WriteFile(const vec<V8Value*>& args)
{
    wstr destPath = config::pluginsDir() + L"\\" + args[0]->asString()->str;
    wstr content = args[1]->asString()->str;
    bool enableAppMode = args[2]->asBool();

    if (PluginFS::WriteFile(destPath, content, enableAppMode)) {
        return V8Value::boolean(true);
    }
    return V8Value::boolean(false);
}

V8Value* native_MkDir(const vec<V8Value*>& args)
{
    wstr pluginsDir = config::pluginsDir();
	wstr pluginName = args[0]->asString()->str;
    wstr relativePath = args[1]->asString()->str;

	if (PluginFS::MkDir(pluginsDir + L"\\" + pluginName, relativePath)) {
		return V8Value::boolean(true);
	}
	return V8Value::boolean(false);
}

V8Value* native_Stat(const vec<V8Value*>& args) {
    wstr destPath = config::pluginsDir() + L"\\" + args[0]->asString()->str;
    if (!std::filesystem::exists(destPath)) {
        return V8Value::undefined();
    }
    PluginFS::FileStat fileStat = PluginFS::Stat(destPath);

    V8Object* v8Obj = V8Object::create();
    v8Obj->set(&L"length"_s, V8Value::number(fileStat.size),V8_PROPERTY_ATTRIBUTE_READONLY);
    v8Obj->set(&L"isDir"_s, V8Value::boolean(fileStat.isDir), V8_PROPERTY_ATTRIBUTE_READONLY);
    v8Obj->set(&L"isFile"_s, V8Value::boolean(fileStat.isFile), V8_PROPERTY_ATTRIBUTE_READONLY);
    v8Obj->set(&L"fileName"_s, V8Value::string(&CefStr{ fileStat.fileName }), V8_PROPERTY_ATTRIBUTE_READONLY);

    return (V8Value*)v8Obj;
}

V8Value* native_ReadDir(const vec<V8Value*>& args) {
	wstr destPath = config::pluginsDir() + L"\\" + args[0]->asString()->str;
	if (!std::filesystem::exists(destPath)) {
		return V8Value::undefined();
	}
	vec<PluginFS::FileStat> fileStats = PluginFS::ReadDir(destPath);

	V8Array* v8Array = V8Array::create(fileStats.size());
    for (int i = 0; i < fileStats.size(); i++)
	{
		PluginFS::FileStat fileStat = fileStats[i];
		V8Object* v8Obj = V8Object::create();
		v8Obj->set(&L"length"_s, V8Value::number(fileStat.size), V8_PROPERTY_ATTRIBUTE_READONLY);
		v8Obj->set(&L"isDir"_s, V8Value::boolean(fileStat.isDir), V8_PROPERTY_ATTRIBUTE_READONLY);
		v8Obj->set(&L"isFile"_s, V8Value::boolean(fileStat.isFile), V8_PROPERTY_ATTRIBUTE_READONLY);
		v8Obj->set(&L"fileName"_s, V8Value::string(&CefStr{ fileStat.fileName }), V8_PROPERTY_ATTRIBUTE_READONLY);
		v8Array->set(i, v8Obj);
	}

	return (V8Value*)v8Array;
}