# Prompt Antigravity — UI Seleção / Criação de Personagem (Fallen Tale)

Cole o bloco abaixo no Antigravity (ou outro gerador de arte).  
**Não invente caminhos novos:** cada arquivo abaixo deve ser substituído **com o mesmo nome e tamanho**.

---

## PROMPT (copiar a partir daqui)

Você é o artista de UI do MMORPG **Fallen Tale** (base Priston Tale).

### Identidade visual (obrigatória)

Siga o **mesmo kit da tela de login** já aprovada no projeto:

- Fantasia medieval sombria / premium
- Pedra escura + metal oxidado (carvão, cinza muito escuro)
- Molduras **bronze / ouro antigo forjado** (bevel, desgaste, não “1px gold”)
- Sem vidro, neon, glow forte, gradiente genérico, plástico, sci-fi
- Tipografia serifada robusta, dourado envelhecido ou branco legível com contorno escuro
- PNG/TGA com **alpha real** quando o asset for overlay (sem fundo xadrez, sem fundo preto/branco sobrando)

Referência de estilo: botões e painéis do login (`window`, `btl` Entrar, `bte` Sair) — mesma família visual.

### Regras técnicas

1. **Substituir arquivo por arquivo** (mesmo nome).
2. **Manter dimensões exatas** listadas.
3. Layout/composição similar ao atual (mesmo “lugar” do texto/ícone), só muda o acabamento.
4. Botões da mesma família: Confirmar / Voltar / Sair / Novo Personagem / Deletar — mesmo metal, só muda o rótulo.
5. Nameplates de classe (MECÂNICO, LUTADOR, etc.): mesmo padrão de moldura; cor de acento pode variar por classe, sem neon.
6. Retratos de cabelo: 3 estilos por classe; pares idle/selecionado (`01`/`02` ou `a`/`b`).
7. Entregar em **TGA 32-bit com alpha** (ou PNG 32-bit que será convertido), nítido, sem pixelização.

### Pasta base no cliente

`C:\Cliente Full\StartImage\login\`  
(subpastas `CharSelect\` e `Moryon\`)

---

### A) Botões compartilhados (`CharSelect\`) — todos **128×32** (exceto ok/cancel)

| Arquivo | Texto na arte | Tamanho |
|---|---|---|
| `Bt_select.tga` | Confirmar | 128×32 |
| `Bt_back.tga` | Voltar | 128×32 |
| `Bt_exit.tga` | Sair | 128×32 |
| `Bt_delet.tga` | Deletar (ou Delete) | 128×32 |
| `bt-Newchar.tga` | Novo Personagem | 128×32 |
| `Bt_ok.tga` | OK / Confirmar (compacto) | 64×32 |
| `Bt_cancel.tga` | Cancelar | 64×32 |

Também: `New_char.tga` **256×64** — faixa/hover “Novo Personagem”.

---

### B) Cromado / títulos (`CharSelect\`)

| Arquivo | Uso | Tamanho |
|---|---|---|
| `title-box.tga` | Caixa de título | 256×128 |
| `title-txt30.tga` | Faixa de títulos (sprite sheet; manter layout UV) | 128×128 |
| `mark_box1.tga` / `mark_box2.tga` | Caixa de texto da tribo | 256×256 / 32×256 |
| `T_cls-box1.tga` / `T_cls-box2.tga` | Caixa de texto da classe | 256×256 / 32×256 |
| `face_bigbox1.tga` / `face_bigbox2.tga` | Moldura grande do nick/rosto | 256×256 / 256×128 |
| `face_box_.tga` / `face_box.tga` | Moldura pequena idle / selecionada | 128×128 |
| `face_box2.tga` | Sublinhado do campo NICK | 64×16 |

---

### C) Tribos (`CharSelect\`)

| Arquivo | Uso | Tamanho |
|---|---|---|
| `mark_T.tga` | Ícone Tempskron | 128×128 |
| `T-tempscron.tga` | Nameplate Tempskron | 128×32 |
| `T-ex_tscr.tga` | Texto/descrição Tempskron | 256×128 |
| `mark_M.tga` | Ícone Morion | 128×128 |
| `T-Morion.tga` | Nameplate Morion | 128×32 |
| `T-ex_mrin.tga` | Texto/descrição Morion | 256×256 |

---

### D) Nameplates de classe — idle `_` + hover (sem `_`)

Tempskron (`CharSelect\`, **128×32** nameplate + **256×256** descrição):

- Mecânico: `T_cls-meca_.tga` / `T_cls-meca.TGA` + `T_chr-meca.tga`
- Lutador: `T_cls-ft_.tga` / `T_cls-ft.TGA` + `T_chr-ft.tga`
- Pikeman: `T_cls-pkm_.tga` / `T_cls-pkm.TGA` + `T_chr-pkm.tga`
- Arqueira: `T_cls-ac_.tga` / `T_cls-ac.TGA` + `T_chr-ac.tga`

Morion:

- Knight: `T-cls-knt_.tga` / `T-cls-knt.TGA` + `T_chr-knt.tga`
- Atalanta: `T_cls-atlt_.tga` / `T_cls-atlt.TGA` + `T_chr-atlt.tga`
- Priestess: `T_cls-prts_.tga` / `T_cls-prts.TGA` + `T_chr-prts.tga`
- Magician: `T_cls-mgc_.tga` / `T_cls-mgc.tga` + `T_chr-mgc.tga`

(Opcionais no disco, se for padronizar: Assassin/Shaman `T_cls-assa*`, `T_cls-sha*`.)

---

### E) Retratos de cabelo / face — **128×128**

Tempskron em `StartImage\login\`:

- Mecânico: `TmA01-01.tga`/`TmA01-02.tga` … `TmA03-01`/`02`
- Lutador: `TmB01-01/02` … `TmB03-01/02`
- Pikeman: `TmC01-01/02` … `TmC03-01/02`
- Arqueira: `TfD01-01/02` … `TfD03-01/02`

Morion em `StartImage\login\Moryon\`:

- Knight: `MmA01-a/b` … `MmA03-a/b`
- Atalanta: `MfB01-a/b` … `MfB03-a/b`
- Priestess: `MfC01-a/b` … `MfC03-a/b`
- Magician: `MmD01-a/b` … `MmD03-a/b`

Convenção: `01`/`a` = idle, `02`/`b` = selecionado (destaque sutil na moldura, sem neon).

---

### F) Painéis de info do personagem (`StartImage\login\`)

| Arquivo | Tamanho |
|---|---|
| `UserCharacterInfo.tga` | 256×128 |
| `CharacterInfo.tga` | 256×128 |
| `BlackBox.tga` | 64×64 (barra/fade) |

### Não redesenhar agora

- Cenário 3D: `game\maps\chrselect\` (ASE/texturas do stage)
- Login de conta: `game\images\login\` (já feito à parte)

### Checklist de entrega

- [ ] Mesmos nomes e tamanhos
- [ ] Família visual igual ao login
- [ ] Botões Confirmar/Voltar/Sair consistentes
- [ ] Alpha limpo nos overlays
- [ ] Retratos idle vs selected distinguíveis

## FIM DO PROMPT
