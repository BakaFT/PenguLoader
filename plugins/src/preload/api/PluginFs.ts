import native from './native';

window.PluginFs = {
    read(path: string) {
        return new Promise<string | undefined>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.ReadFile(pluginName + "/" + path);
            resolve(result);
        });
    },
    write(path: string, content: string, enableAppendMode: boolean) {
        return new Promise<boolean>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.WriteFile(pluginName + "/" + path, content, enableAppendMode);
            resolve(result);
        });
    }
};