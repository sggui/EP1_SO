#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1) // Para garantir que a estrutura seja compactada sem padding

// Definição da estrutura MBR
typedef struct {
    uint8_t boot_indicator;  // Indicador de boot
    uint8_t start_head;      // Cabeça de início
    uint8_t start_sector;    // Setor de início
    uint8_t start_cylinder;  // Cilindro de início
    uint8_t partition_type;  // Tipo de partição
    uint8_t end_head;        // Cabeça final
    uint8_t end_sector;      // Setor final
    uint8_t end_cylinder;    // Cilindro final
    uint32_t start_sector_lba;  // Endereço do setor inicial (LBA)
    uint32_t num_sectors;    // Número de setores
} PartitionEntry;

typedef struct {
    uint8_t boot_code[446];        // Código de boot
    PartitionEntry partitions[4];  // Tabela de partições (4 entradas)
    uint16_t signature;            // Assinatura (deve ser 0xAA55)
} MBR;

#pragma pack(pop)

int main() {
    FILE *file = fopen("mbr.bin", "rb");
    if (!file) {
        perror("Erro ao abrir o arquivo MBR");
        return 1;
    }

    MBR mbr;

    // Ler o conteúdo do arquivo mbr.bin
    fread(&mbr, sizeof(MBR), 1, file);
    fclose(file);

    // Verificar a assinatura do MBR
    if (mbr.signature != 0xAA55) {
        printf("Assinatura MBR inválida: 0x%x\n", mbr.signature);
        return 1;
    }

    printf("Assinatura MBR: 0x%x\n", mbr.signature);
    printf("Partições:\n");

    // Iterar sobre as partições
    for (int i = 0; i < 4; i++) {
        PartitionEntry *p = &mbr.partitions[i];
        printf("Partição %d:\n", i + 1);
        printf("  Tipo de partição: 0x%x\n", p->partition_type);
        printf("  Setor inicial (LBA): %u\n", p->start_sector_lba);
        printf("  Número de setores: %u\n", p->num_sectors);
        printf("  Indicador de boot: 0x%x\n", p->boot_indicator);
    }

    return 0;
}

