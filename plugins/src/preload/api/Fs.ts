import native from './native';

window.Fs = {
    readFile(path: string) {
        // ignore top-level js
        const pluginEntry = new Error().stack?.match(/(?:http|https):\/\/[^\s]+\.js/g)?.[0]
        const match = pluginEntry?.match(/\/([^/]+)\/index\.js$/)
        const pluginName = match ? match[1] : null;

        return native.ReadFile(pluginName + "/" + path);
    },
    readFileAsync(path: string) {
        return new Promise<string | undefined>((resolve) => {
            const pluginEntry = new Error().stack?.match(/(?:http|https):\/\/[^\s]+\.js/g)?.[0]
            const match = pluginEntry?.match(/\/([^/]+)\/index\.js$/)
            const pluginName = match ? match[1] : null;

            const result = native.ReadFile(pluginName + "/" + path);
            resolve(result);

        });
    },
    writeFile(path: string, content: string, enableAppendMode: boolean) {
        const pluginEntry = new Error().stack?.match(/(?:http|https):\/\/[^\s]+\.js/g)?.[0]
        const match = pluginEntry?.match(/\/([^/]+)\/index\.js$/)
        const pluginName = match ? match[1] : null;

        return native.WriteFile(pluginName + "/" + path, content, enableAppendMode);
    },
    writeFileAsync(path: string, content: string, enableAppendMode: boolean) {
        return new Promise<boolean>((resolve) => {
            const pluginEntry = new Error().stack?.match(/(?:http|https):\/\/[^\s]+\.js/g)?.[0]
            const match = pluginEntry?.match(/\/([^/]+)\/index\.js$/)
            const pluginName = match ? match[1] : null;

            const result = native.WriteFile(pluginName + "/" + path, content, enableAppendMode);
            resolve(result);
        });
    }
};