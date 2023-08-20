import native from './native';

window.PluginFs = {
    read(path: string) {
        return new Promise<string | undefined>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.ReadFile(pluginName + "/" + path);
            resolve(result);
        });
    },
    write(path: string, content: string, enableAppendMode: boolean = false) {
        return new Promise<boolean>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.WriteFile(pluginName + "/" + path, content, enableAppendMode);
            if(!result){
                console.log("PluginFS.write failed. Please ask the plugin developer for help.")
            }
            resolve(result);
        });
    },
    mkdir(path: string) {
        return new Promise<boolean>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.MkDir(pluginName!,path);
            resolve(result);
        });
    },
    stat(path: string) {
        return new Promise<FileStat | undefined>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.Stat(pluginName + "/" + path);
            resolve(result);
        });
    },
    readDir(path: string) {
        return new Promise<FileStat[] | undefined>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.ReadDir(pluginName + "/" + path);
            resolve(result);
        });
    },
    rm(path: string, recursively: boolean = false) {
        return new Promise<number>((resolve) => {
            const pluginName = getScriptPath()?.match(/\/([^/]+)\/index\.js$/)?.[1]
            const result = native.Remove(pluginName + "/" + path, recursively);
            resolve(result);
        });
    }
}