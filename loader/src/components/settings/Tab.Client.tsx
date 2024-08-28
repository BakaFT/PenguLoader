import { Component } from 'solid-js'
import { useConfig } from '~/lib/config'
import { CheckOption, OptionSet } from './templates'
import { useI18n } from '~/lib/i18n'

export const TabClient: Component = () => {

  const i18n = useI18n()
  const { client } = useConfig()

  return (
    <div class="space-y-4">

      <p class="text-sm text-neutral-400">{i18n.t("settings.leagueclient.restart_to_take_effect")}</p>

      <OptionSet name={i18n.t("settings.leagueclient.hotkeys_title")}>
        <CheckOption
          caption={i18n.t("settings.leagueclient.hotkeys_caption")}
          message={i18n.t("settings.leagueclient.hotkeys_message")}
          checked={client.use_hotkeys()}
          onChange={client.use_hotkeys}
        />
        <div class="space-y-2 ml-8 aria-disabled:opacity-50" aria-disabled={!client.use_hotkeys()}>
          <div class="flex items-center space-x-2">
            <kbd class="px-2 py-0.5 rounded-sm text-xs bg-neutral-500/30">Ctrl Shift R</kbd>
            <p class="text-sm text-neutral-400">{i18n.t("settings.leagueclient.hotkeys_reload_the_client")}</p>
          </div>
          <div class="flex items-center space-x-2">
            <kbd class="px-2 py-0.5 rounded-sm text-xs bg-neutral-500/30">Ctrl Shift Enter</kbd>
            <p class="text-sm text-neutral-400">{i18n.t("settings.leagueclient.hotkeys_restart_the_ux")}</p>
          </div>
          <div class="flex items-center space-x-2 aria-disabled:line-through" aria-disabled={!client.use_devtools()}>
            <kbd class="px-2 py-0.5 rounded-sm text-xs bg-neutral-500/30">Ctrl Shift I</kbd>
            <span>/</span>
            <kbd class="px-2 py-0.5 rounded-sm text-xs bg-neutral-500/30">F12</kbd>
            <p class="text-sm text-neutral-400">{i18n.t("settings.leagueclient.hotkey_open_chrome_devtools")}</p>
          </div>
        </div>
      </OptionSet>

      <OptionSet name={i18n.t("settings.leagueclient.tweaks_title")}>
        <CheckOption
          caption={i18n.t("settings.leagueclient.optimized_client_caption")}
          message={i18n.t("settings.leagueclient.optimized_client_message")}
          checked={client.optimed_client()}
          onChange={client.optimed_client}
        />
        <CheckOption
          caption={i18n.t("settings.leagueclient.super_potato_caption")}
          message={i18n.t("settings.leagueclient.super_potato_message")}
          checked={client.super_potato()}
          onChange={client.super_potato}
        />
        <CheckOption
          caption={i18n.t("settings.leagueclient.silent_mode_caption")}
          message={i18n.t("settings.leagueclient.slient_mode_message")}
          checked={client.silent_mode()}
          onChange={client.silent_mode}
        />
      </OptionSet>

      <OptionSet name={i18n.t("settings.leagueclient.developer_title")}>
        <CheckOption
          caption={i18n.t("settings.leagueclient.developer_tools_caption")}
          message={i18n.t("settings.leagueclient.developer_tools_message")}
          checked={client.use_devtools()}
          onChange={client.use_devtools}
        />
        <CheckOption
          caption={i18n.t("settings.leagueclient.insecure_mode_caption")}
          message={i18n.t("settings.leagueclient.insecure_mode_message")}
          checked={client.insecure_mode()}
          onChange={client.insecure_mode}
        />
        <CheckOption
          caption={i18n.t("settings.leagueclient.riotclient_api_caption")}
          message={i18n.t("settings.leagueclient.riotclient_api_message")}
          checked={client.use_riotclient()}
          onChange={client.use_riotclient}
        />
        <CheckOption
          caption={i18n.t("settings.leagueclient.allow_proxy_caption")}
          message={i18n.t("settings.leagueclient.allow_proxy_message")}
          checked={client.use_proxy()}
          onChange={client.use_proxy}
        />
      </OptionSet>

    </div>
  )
}
