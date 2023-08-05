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
        CefStr content_cef_string{ content };
        return V8Value::string(&content_cef_string);
	}
    return V8Value::undefined();
}
