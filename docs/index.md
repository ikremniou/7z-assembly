---
layout: home

hero:
  name: "7zip Assembly"
  text: "How to build 7zip(7z) archiver format plugins"
  tagline: This project is a light guide on how to work with 7zip archiver source code and build comprehensive plugins for 7zip File Manager.

features:
  - title: Beginning
    details: Learn where to find the latest version of the 7zip source code, how to debug and what is the structure of the plugin host and its API
    link: /beginning
  - title: Plugin Interface
    details: Explore the contract that your plugin must implement to serve as an archiver
    link: /plugin-api-def
  - title: Implementation of the sample archiver(SZ) format plugin
    details: We will implement an archiver with hardcoded file structure. We will allow files to be opened and the archive can be extracted.
    link: /sz-impl
  - title: Implementation of the extended version of the format(SZE)
    details: We will explore a bit more the plugin API and try implementing an archive that will allow files to be inserted, deleted, and modified in-place.
    link: /sze-impl
---

