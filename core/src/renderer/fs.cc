#include "commons.h"
#include <fstream>
#include <vector>
#include <locale>

template <typename T>
class StreamGuard
{
public:
    StreamGuard(T &stream) : stream_(stream) {}
    ~StreamGuard()
    {
        if (stream_.is_open())
        {
            stream_.close();
        }
    }

private:
    T &stream_;
};

static vec<char> ReadFileBinary(wstr path)
{
    std::fstream inputStream(path, std::ios::binary);

    if (!inputStream.good())
    {
        return {};
    }

    StreamGuard<std::fstream> guard(inputStream);

    inputStream.seekg(0, std::ios::end);
    std::streampos fileSize = inputStream.tellg();
    inputStream.seekg(0, std::ios::beg);

    vec<char> buffer(fileSize);
    inputStream.read(buffer.data(), fileSize);

    return buffer;
}

static vec<wstr> ReadFile(wstr path)
{
    std::wfstream inputStream;
    inputStream.open(path, std::ios::in);

    if (!inputStream.good())
    {
        return {};
    }

    StreamGuard<std::wfstream> guard(inputStream);

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

static bool WriteFile(wstr path, wstr *content,bool enableAppendMode)
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

    StreamGuard<std::wfstream> guard(outputStream);

    std::locale utf8_locale = std::locale("en_US.UTF-8");
    outputStream.imbue(utf8_locale);
    
    outputStream << *content;
    if (outputStream.fail() || outputStream.bad())
    {
        return false; 
    }

    return true;
}

V8Value* native_ReadFile(const vec<V8Value*>& args)
{
    wstr destPath = config::pluginsDir() + L"\\" + args[0]->asString()->str;
    if(utils::isFile(destPath)){
        vec<wstr> lines = ReadFile(destPath);
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

    if (WriteFile(destPath, &content, enableAppMode)) {
        return V8Value::boolean(true);
    }
    return V8Value::boolean(false);
}
