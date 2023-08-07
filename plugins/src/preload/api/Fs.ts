import native from './native';

window.Fs = {
    readFile(path: string) {
        // ignore top-level js
        const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
        return native.ReadFile(pluginName + "/" + path);
    },
    readFileAsync(path: string) {
        return new Promise<string | undefined>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.ReadFile(pluginName + "/" + path);
            resolve(result);
        });
    },
    writeFile(path: string, content: string, enableAppendMode: boolean) {
        const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
        return native.WriteFile(pluginName + "/" + path, content, enableAppendMode);
    },
    writeFileAsync(path: string, content: string, enableAppendMode: boolean) {
        return new Promise<boolean>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.WriteFile(pluginName + "/" + path, content, enableAppendMode);
            resolve(result);
        });
    }
};