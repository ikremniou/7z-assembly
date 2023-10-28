---
# https://vitepress.dev/reference/default-theme-home-page
layout: home

hero:
  name: "7zip Assembly"
  text: "Archiver plugin documentation"
  tagline: This project is a light guide on how to work with 7zip archiver source code and create File Manager format plugins.

features:
  - title: Beginning
    details: Learn where to find the latest version of the 7zip source code, how to debug and what is the structure of the plugin host and it's API
    link: /beginning
  - title: Implementation of the sample archiver(SZ) format plugin
    details: We will implement archiver with hardcoded file structure. We will allow files to be opened and archive can be extracted.
    link: /sz-implementation
  - title: Implementation of the extended version of the format(SZE)
    details: We will explore a bit more the plugin API and try implementing archive that will allow files to be inserted, deleted and modified in-place.
    link: /sze-implementation
---

