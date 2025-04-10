# 🇫🇷 Conjugueur de Verbes Français en C

Un programme en **C** permettant de conjuguer automatiquement des verbes français au :
- **présent**
- **imparfait**
- **passé simple**
- **futur simple**

En tenant compte :
- des **trois groupes de verbes** (1er, 2e, 3e),
- des **formes pronominales** (ex : *se laver*, *s’endormir*...),
- des **accents** (conversion ASCII ↔ UTF-8),
- de nombreuses **exceptions du 3e groupe**.

---

## ✨ Fonctionnalités

- 🔍 Détection automatique du **groupe** du verbe.
- 🔁 Gestion des **verbes pronominaux**.
- 🔤 Conversion entre **caractères accentués** et **équivalents ASCII**.
- 🧠 Gestion des **formes irrégulières** et **cas particuliers**.
- 📄 Lecture d’un fichier `verbes.txt` pour lister les verbes reconnus.

---

## 🚀 Utilisation

### 1. Compilation

```bash
gcc -o conjugueur conjugueur.c
