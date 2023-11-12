import { defineConfig } from 'vitepress'

// https://vitepress.dev/reference/site-config
export default defineConfig({
  title: "7zip Assembly",
  description: "Archiver plugin documentation",
  base: "/7z-assembly/",
  themeConfig: {
    // https://vitepress.dev/reference/default-theme-config
    nav: [
      { text: 'Home', link: '/' },
      { text: 'Documentation', link: '/beginning' }
    ],

    sidebar: [
      {
        text: 'Beginning',
        items: [
          { text: 'Initial setup', link: '/beginning' },
          { text: 'How to compile', link: '/how-to-compile' },
          { text: 'Plugin system', link: '/plugin-system' }
        ]
      },
      {
        text: 'API',
        items: [
            { text: 'Archiver API', link: '/plugin-api-def' },
        ]
      },
      {
        text: 'Implementation',
        items: [
            { text: 'Infrastructure', link: '/infrastructure' },
            { text: 'Implementing SZ', link: '/sz-impl' },
            { text: 'Implementing SZE', link: '/sze-impl' },
        ]
      }
    ],

    socialLinks: [
      { icon: 'github', link: 'https://github.com/ikremniou/7z-assembly' },
      { icon: 'linkedin', link: 'https://linkedin.com/in/ilya-kremniou-b747b11aa' }
    ]
  }
})
