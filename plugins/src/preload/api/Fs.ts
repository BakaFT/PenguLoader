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
            if (typeof result === 'string') {
                resolve(result);
            }
        });
    }
};