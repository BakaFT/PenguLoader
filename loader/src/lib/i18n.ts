import { createRoot } from 'solid-js'
import { createStore } from 'solid-js/store'
import translations from '../../translations.json'
import { useConfig } from './config'

const EN = translations.languages[0]

// @ts-ignore
type NestedTranslationKey<T> = {
  [K in keyof T]: T[K] extends object
  ? `${string & K}.${NestedTranslationKey<T[K]>}`
  : `${string & K}`
}[keyof T]

type PenguTranslationKey = NestedTranslationKey<typeof EN.translations>

type TranslationMap = Record<string, any>

const _i18n = createRoot(() => {
  const [current, set] = createStore<TranslationMap>({ ...EN.translations })

  const languages = translations.languages.map((x) => ({
    id: x.id,
    name: x.name,
  }))

  const switchTo = (id: string) => {
    for (const lang of translations.languages) {
      if (lang.id === id) {
        set({ ...lang.translations })
      }
    }
  }


  const text = (key: PenguTranslationKey): string => {
    const keys = key.split('.')
    let value: any = current

    for (const k of keys) {
      value = value[k]
      if (value === undefined) {
        return `{{${key}}}`
      }
    }

    return typeof value === 'string' ? value : `{{${key}}}`
  }

  return {
    languages,
    switchTo,
    t: text,
  }
})

export const useI18n = () => {
  _i18n.switchTo(useConfig().app.language())
  return _i18n
}
