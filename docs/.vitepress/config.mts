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
            { text: 'Plugin Interface', link: '/plugin-api-def' },
            { text: 'IInArchive', link: '/plugin-api-inarc' },
            { text: 'IOutArchive', link: '/plugin-api-outarc' },
            { text: 'IArchiveOpenCallback', link: '/plugin-api-open-callback' },
            { text: 'IArchiveExtractCallback', link: '/plugin-api-extract-callback' },
            { text: 'IArchiveUpdateCallback', link: '/plugin-api-update-callback' },
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
