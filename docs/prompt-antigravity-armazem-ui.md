# Prompt Antigravity — Título ImGui do Armazém (Fallen Tale)

Cole o bloco abaixo no Antigravity (Gemini).  
O Cursor **não** gera PNG. Grave o arquivo no **Cliente Full**.

O F5 lê o cliente, não a source.

---

## PROMPT (copiar a partir daqui)

Você é o artista de UI do MMORPG **Fallen Tale** (base Priston Tale).

### Identidade visual (obrigatória)

Siga o **mesmo kit da tela de login** já aprovada:

- Fantasia medieval sombria / premium
- Pedra escura + metal oxidado (carvão, cinza muito escuro)
- Molduras **bronze / ouro antigo forjado** (bevel, desgaste, não “1px gold”)
- Sem vidro, neon, glow forte, gradiente genérico, plástico, sci-fi
- Tipografia serifada robusta, dourado envelhecido ou branco com contorno escuro
- PNG com **alpha real** se houver recorte; o fundo da placa é opaco `RGB(20,24,32)`

Referências concretas (abrir e imitar o metal, não inventar família nova):

- Bronze envelhecido igual `C:\Cliente Full\game\images\login\window.png`, `btl.png` (Entrar) e `bte.png` (Sair)
- Botões com texto centralizado: `C:\Cliente Full\StartImage\login\CharSelect\Bt_select.tga` (Confirmar, 128×32) — só como referência de metal; **não** gere botões nesta tarefa
- Título ImGui 400×64: `C:\Cliente Full\game\images\quest\desafios.png` (ícone + nome na mesma placa de ouro fosco)

Textura de bronze envelhecido igual `window.png` / `btl.png` do login.

### Pasta destino

`C:\Cliente Full\game\images\warehouse\`

Crie a pasta se não existir. Cópia opcional no source: `C:\Source Priston\Source Priston\game\images\warehouse\` (não substitui o cliente).

### Arquivos (dimensões exatas)

| Arquivo | Texto na arte | Tamanho | Notas |
|---|---|---|---|
| `armazem.png` | ARMAZÉM (com acento) | **400×64** | Uma placa só: ícone de baú/cofre à esquerda (silhueta na altura das maiúsculas, mesmo metal das letras) + o título. Fundo opaco `RGB(20,24,32)`. Recorte o desenho **dentro** dos 400×64. Ouro fosco, entalhes internos em bronze escuro. Sem halo, bloom, brilho branco ou ponto especular forte. |

O C++ carrega exatamente: `game\images\warehouse\armazem.png`

### Regras técnicas

1. Substituir **mesmo nome**. Não inventar path.
2. Manter **dimensões exatas** 400×64. Outro tamanho vira selo ilegível (a escala usa a imagem inteira).
3. Canvas 400×64, desenho recortado, fundo `RGB(20,24,32)` — o header ImGui usa essa mesma cor, sem emenda no DX9.
4. Um arquivo só. O código reduz com proporção para a faixa do header (~210–320×32–40).
5. PNG 32-bit. Fundo da placa opaco (não transparente no retângulo 400×64).

### Não redesenhar agora

- Não gerar PNG de janela inteira, moldura, 9-slice ou fundo tileado — o cromado é desenhado no ImDrawList (`DrawPlayerWindowChrome`).
- Não gerar botões 1 / 2 / 3, VOLTAR, X, campo de busca — são botões ImGui de texto.
- Não gerar `Image\SinImage\Shopall\shop-1.bmp` (compartilhado com loja NPC, aging, caravana).
- Não redesenhar login (`window.png`, `btl`, `bte`) nem seleção de personagem.
- Inventário continua HUD de pedra; não mexer nele.

### Checklist de entrega

- [ ] Nome exato `armazem.png` em `C:\Cliente Full\game\images\warehouse\`
- [ ] 400×64, desenho recortado, fundo `RGB(20,24,32)`
- [ ] Ícone de baú + “ARMAZÉM” na mesma placa, família visual igual a `desafios.png` + bronze do login
- [ ] Sem glow / neon / vidro

## FIM DO PROMPT
