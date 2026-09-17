-- Fallen Tale / Source Priston
-- Baú (warehouse) no UserDB.
-- Cole no SSMS conectado em UserDB. O C++ NÃO cria estas tabelas no boot.
--
-- 5 páginas (0..4) e 300 slots (0..299) por página.
-- O jogo libera UnlockedPages = 3 no começo.
-- Unique Head+ChkSum por conta bloqueia duplicata (poção/ouro usam Head=0 e ficam de fora).

USE UserDB;
GO

IF OBJECT_ID(N'dbo.Warehouse', N'U') IS NULL
BEGIN
	CREATE TABLE dbo.Warehouse (
		AccountID        VARCHAR(32)  NOT NULL,
		Money            INT          NOT NULL CONSTRAINT DF_Warehouse_Money DEFAULT (2023),
		WeightMax        INT          NOT NULL CONSTRAINT DF_Warehouse_WeightMax DEFAULT (8000),
		UnlockedPages    INT          NOT NULL CONSTRAINT DF_Warehouse_Unlocked DEFAULT (3),
		Revision         INT          NOT NULL CONSTRAINT DF_Warehouse_Revision DEFAULT (1),
		ImportedFromWar  INT          NOT NULL CONSTRAINT DF_Warehouse_Imported DEFAULT (0),
		UpdatedAt        DATETIME     NOT NULL CONSTRAINT DF_Warehouse_Updated DEFAULT (GETDATE()),
		CONSTRAINT PK_Warehouse PRIMARY KEY (AccountID),
		CONSTRAINT CK_Warehouse_Unlocked CHECK (UnlockedPages >= 1 AND UnlockedPages <= 5),
		CONSTRAINT CK_Warehouse_Money CHECK (Money >= 0),
		CONSTRAINT CK_Warehouse_WeightMax CHECK (WeightMax >= 0)
	);
END
GO

IF OBJECT_ID(N'dbo.WarehouseItem', N'U') IS NULL
BEGIN
	CREATE TABLE dbo.WarehouseItem (
		AccountID  VARCHAR(32)    NOT NULL,
		Page       TINYINT        NOT NULL,
		Slot       SMALLINT       NOT NULL,
		GridX      SMALLINT       NOT NULL,
		GridY      SMALLINT       NOT NULL,
		ItemBlob   VARBINARY(2048) NOT NULL,
		ItemCode   INT            NOT NULL,
		Head       INT            NOT NULL,
		ChkSum     INT            NOT NULL,
		CONSTRAINT PK_WarehouseItem PRIMARY KEY (AccountID, Page, Slot),
		CONSTRAINT FK_WarehouseItem_Account FOREIGN KEY (AccountID)
			REFERENCES dbo.Warehouse (AccountID),
		CONSTRAINT CK_WarehouseItem_Page CHECK (Page >= 0 AND Page <= 4),
		CONSTRAINT CK_WarehouseItem_Slot CHECK (Slot >= 0 AND Slot <= 299)
	);

	CREATE INDEX IX_WarehouseItem_AccountPage
		ON dbo.WarehouseItem (AccountID, Page);

	CREATE UNIQUE INDEX UX_WarehouseItem_HeadChk
		ON dbo.WarehouseItem (AccountID, Head, ChkSum)
		WHERE Head <> 0 AND ChkSum <> 0;
END
GO
