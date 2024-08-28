import { Component, createSignal, onMount } from 'solid-js'
import { useRoot } from '~/lib/root'
import { StoreManager } from '~/lib/store'
import { useI18n } from '~/lib/i18n'

export const PluginStore: Component = () => {
  
  const i18n = useI18n()  
  const { setStore } = useRoot()
  // const [loading, setLoading] = createSignal(true)

  // onMount(async () => {
  //   const plugins = await StoreManager.fetchPlugins()
  // })

  return (
    <div class="h-full flex flex-col p-4">
      <div class="flex-1 flex flex-col items-center">
        <div class="m-auto text-center space-y-4">
          <p class="text-lg m-auto">{i18n.t('store.soon')}</p>
          <button
            class="inline-flex gap-1 items-center text-sm border border-foreground/10 rounded-sm px-3 py-1 hover:bg-foreground hover:text-background"
            tabIndex={-1} onClick={() => setStore(false)}
          >{i18n.t('store.go_back')}</button>
        </div>
      </div>
    </div>
  )
}