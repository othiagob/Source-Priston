# Template — prompt Antigravity (Gemini)

Copie para `docs/prompt-antigravity-<tela>-ui.md` e preencha.  
O Cursor **nao** gera os arquivos de arte. Destino: **Cliente Full**.

Modelos preenchidos: `prompt-antigravity-charselect-ui.md`,
`prompt-antigravity-armazem-ui.md`.

---

## PROMPT (copiar a partir daqui)

Voce e o artista de UI do MMORPG **Fallen Tale** (base Priston Tale).

### Identidade visual (obrigatoria)

Siga o **mesmo kit da tela de login** ja aprovada:

- Fantasia medieval sombria / premium
- Pedra escura + metal oxidado (carvao, cinza muito escuro)
- Molduras **bronze / ouro antigo forjado** (bevel, desgaste, nao "1px gold")
- Sem vidro, neon, glow forte, gradiente generico, plastico, sci-fi
- Tipografia serifada robusta, dourado envelhecido ou branco com contorno escuro
- PNG/TGA com **alpha real** em overlay (sem fundo xadrez, sem fundo preto/branco)

Referencias concretas (abrir e imitar o metal, nao inventar familia nova):

- Bronze envelhecido: `C:\Cliente Full\game\images\login\window.png`, `btl.png` (Entrar), `bte.png` (Sair)
- Botoes com texto centralizado: `C:\Cliente Full\StartImage\login\CharSelect\Bt_select.tga` (Confirmar, 128x32) e a familia Voltar/Sair
- Titulo ImGui 400x64: `C:\Cliente Full\game\images\quest\desafios.png` (icone + nome na mesma placa)

[Se HUD de pedra:] nao usar o cromado ImGui. Familia de `Image\SinImage\Shopall\shop-1.bmp`.

### Pasta destino

`C:\Cliente Full\<path que o C++ ja carrega>\`

### Arquivos (dimensoes exatas)

| Arquivo | Texto na arte | Tamanho | Notas |
|---|---|---|---|
| `nome.ext` | ROTULO | LxA | idle / hover se houver par `_` |

### Regras tecnicas

1. Substituir **mesmo nome**. Nao inventar path.
2. Manter **dimensoes exatas**. Outro tamanho desloca hitbox ou vira selo.
3. Layout no mesmo "lugar" da peca atual; so muda o acabamento.
4. Titulo ImGui: canvas **400x64**, desenho recortado, fundo `RGB(20,24,32)` se for placa de header.
5. Overlay: alpha limpo. HUD legado BMP: opaco, sem alpha.

### Nao redesenhar agora

- [listar: login de conta, shop-1.bmp compartilhado, cenario 3D, etc.]

### Checklist de entrega

- [ ] Mesmos nomes e tamanhos
- [ ] Familia visual igual ao login / referencia citada
- [ ] Texto centralizado nos botoes (se houver rotulo na textura)
- [ ] Alpha limpo (se overlay)

## FIM DO PROMPT
