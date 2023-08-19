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
        bool isDir;
        int size;
    };

    static PluginFS::FileStat Stat(wstr path)
	{
        return PluginFS::FileStat{
            std::filesystem::is_directory(path),
            static_cast<int>(std::filesystem::file_size(path))
        }; 
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
    return (V8Value*)v8Obj;
}