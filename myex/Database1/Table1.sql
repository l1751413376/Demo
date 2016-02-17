CREATE TABLE [dbo].[Table1]
(
	[Id] INT NOT NULL PRIMARY KEY DEFAULT 1 IDENTITY, 
    [Title] NVARCHAR(300) NOT NULL DEFAULT ''
)

GO
EXEC sp_addextendedproperty @name = N'MS_Description',
    @value = N'主键',
    @level0type = N'SCHEMA',
    @level0name = N'dbo',
    @level1type = N'TABLE',
    @level1name = N'Table1',
    @level2type = N'COLUMN',
    @level2name = N'Id'