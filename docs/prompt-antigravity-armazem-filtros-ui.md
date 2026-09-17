# Prompt Antigravity — Ícones de filtro do Armazém (Fallen Tale)

Cole o bloco **PROMPT** no Antigravity (Gemini).
O Cursor **não** gera PNG. Grave no **Cliente Full**. O F5 lê o cliente, não a source.

São **só ícones 32×32** para a faixa de filtros ao lado da busca. Não redesenhar `frame.png` nem o título `armazem.png`.

---

## PROMPT (copiar a partir daqui)

Você é o artista de UI do MMORPG **Fallen Tale** (base Priston Tale).

Tarefa: **7 ícones 32×32** para filtros do Armazém. Mesmo ouro forjado dos títulos ImGui. Fundo **alpha 0**. Sem texto nas peças (o jogo mostra tooltip).

### Identidade visual

- Fantasia medieval sombria / premium
- Bronze / ouro antigo forjado (bevel, desgaste, não “1px gold”)
- Sem vidro, neon, glow, bloom, plástico, sci-fi
- PNG 32-bit, alpha real (sem xadrez, sem fundo preto/branco)

Metal igual `C:\Cliente Full\game\images\login\window.png` / `btl.png`.
Ícone 3D no espírito de `C:\Cliente Full\game\images\warehouse\armazem.png` (baú) e `C:\Cliente Full\game\images\postbox\distribuidor.png`.

### Pasta destino

`C:\Cliente Full\game\images\warehouse\`

Cópia opcional: `C:\Source Priston\Source Priston\game\images\warehouse\`

O C++ carrega exatamente estes nomes:

| Arquivo | Ícone (silhueta 3D, ouro forjado) |
|---|---|
| `filter-all.png` | Grade 3×3 / todos os itens |
| `filter-weapons.png` | Espada |
| `filter-armors.png` | Peitoral / armadura |
| `filter-boots.png` | Bota |
| `filter-gloves.png` | Luva |
| `filter-premiums.png` | Coroa / joia premium |
| `filter-sheltons.png` | Cristal / sheltom facetado |

Todos **32×32 px**, fundo alpha 0, recorte interno com ~3 px de folga (o botão já tem bezel). Sem letras. Sem moldura de janela.

### Não redesenhar

- `frame.png`, `armazem.png`, login, inventário `shop-1.bmp`, 9-slice.

### Checklist

- [ ] Sete PNG 32×32 em `C:\Cliente Full\game\images\warehouse\`
- [ ] Alpha 0 no fundo, ouro forjado, sem texto
- [ ] Mesmos nomes da tabela

## FIM DO PROMPT
