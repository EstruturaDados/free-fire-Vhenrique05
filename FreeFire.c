#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire

// define constantes para padronização no código
#define MAX_STRING 25
#define MAX_ITEMS 10

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// struct para armazenar informações de itens
typedef struct {
	char nome[MAX_STRING];
	char tipo[MAX_STRING];

	// usei "unsigned int" para que a quantidade não possa ser negativa
	unsigned int quantidade;
} Item;

const Item ItemNulo = {"", "", 0};


// Vetor mochila:
Item* mochila;

// quantidade de itens que a mochila possui
unsigned int mochila_quantidade = 0;

// função para alocação na memória
bool alocarMochila();

// função para liberação da memória
void limparMemoriaMochila();

// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela();

// função usada para comparar 2 itens
bool compararItem(const Item* a, const Item* b);

// função que serve para imprimir as informações de um item
void imprimirItem(const Item* item, bool detalhado);

// Apresenta o menu principal ao jogador, com destaque para status da ordenação.
void exibirMenu();

// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".
void inserirItem();

// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.
void removerItem();

// função que organiza a mochila usando o algoritmo bubble-sort
void organizar_mochila(int i_inicio, int i_final);

// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens();

// função para busca de items por nome
void procurarItem();

// ----------------------------------------------------------------

// função para alocação dinâmica do vetor da mochila na memória
// returna se a alocação foi um sucesso
bool alocarMochila() {
	mochila = (Item*)calloc(MAX_ITEMS, sizeof(Item));
	// se falhar, mochila será NULL
	return mochila != NULL;
}

void limparMemoriaMochila() {
	free(mochila);
}

void limparTela() {
	printf("\n\n\n\n\n\n\n\n\n\n");
}

bool compararItem(const Item* a, const Item* b) {
	return
		strcmp(a->nome, b->nome) == 0 &&
		strcmp(a->tipo, b->tipo) == 0 &&
		a->quantidade == b->quantidade;
}

void imprimirItem(const Item* item, bool detalhado) {
	printf("[>] Nome: %s\n", item->nome);

	// se a opção "detalhado" for verdadeira,
	// ele mostra os dados detalhados
	if (detalhado) {
		printf("[>] Tipo: %s\n", item->tipo);
		printf("[>] Quantidade: %d\n", item->quantidade);
	}
}

void inserirItem() {
	// para evitar o uso de memória desnecessário,
	// verifico primeiro se há espaço na mochila para
	// só depois definir variáveis/dados
	if (mochila == NULL) {
		return;
	}

	if (mochila_quantidade >= MAX_ITEMS) {
		printf("[!] A mochila está cheia!\n");
		return;
	}

	// variável para armazenar os dados informados pelo usuário
	Item item;

	printf("--------[Inserir Item]--------\n");

	// eu utilizo o " %24[^\n]" para ler até 24 caracteres (mais o caráctere \0, para terminação de string)
	// e permitindo textos com espaço, terminando a linha no Enter (\n)
	printf("\nInsira o nome do item:\n>> ");
	scanf(" %24[^\n]", &item.nome);

	printf("\nInsira o tipo do item:\n>> ");
	scanf(" %24[^\n]", &item.tipo);

	printf("\nInsira a quantidade de itens:\n>> ");
	scanf("%d", &item.quantidade);


	// variável para armazenar a opção selecionada
	unsigned int escolha = 0;
	do {
		limparTela();

		printf("------------------------------\n");
		printf("[*] Os dados estão corretos?\n");
		imprimirItem(&item, true);
		printf("------------------------------\n");
		printf(" [1] - Salvar dados\n");
		printf(" [0] - Cancelar\n");
		printf("------------------------------\n");

		printf(">> ");
		scanf("%d", &escolha);
		printf("\n");

		switch (escolha) {
		case 0:
			printf("[*] Cancelando...\n");
			return;

		case 1:
			printf("[*] Salvando dados...\n");
			break;

		default:
			printf("[!] Opção inválida\n");
			break;
		}
	} while (escolha != 1);

	// variável para armazenar espaço disponível no vetor mochila
	int indice_vago = -1;
	for (int i = 0; i < MAX_ITEMS; i++) {
		if (compararItem(&mochila[i], &ItemNulo)) {
			indice_vago = i;
			break;
		}
	}

	if (indice_vago == -1) {
		printf("[!] Falha ao procurar espaço vago dentro da mochila!\n");
		return;
	}

	mochila[indice_vago] = item;

	if (&mochila[indice_vago] != NULL) {
		printf("[*] Item adicionado com sucesso!\n");
		mochila_quantidade++;
	} else {
		printf("[!] Falha ao inserir item dentro da mochila!\n");
	}
}


/*
 * Função que organiza a mochila usando o algoritmo: bubble-sort
 * como funciona?
 * - ele analisa a lista do alcance "i_inicio" até "i_final",
 *   pulando os números já organizados (definido por "j")
 * - quando ele chegar ao final da lista, ele considera o último
 *   número como "organizado"
 * - ele repete o mesmo processo nas próximas sub-listas
 *
 * OBS: Eu não sabia como implementar no código um sistema simples
 *      que me permitisse gerenciar o vetor mochila, sem deixar
 *      valores nulos no meio da lista.
 *
 *      Eu acabei fazendo essa função que realiza a conferência
 *      dos valores, e quando terminei, descobri que ele era uma
 *      implementação do algoritmo bubble-sort.
 *
 */
void organizar_mochila(const int i_inicio, const int i_final) {
	// define quantidade de vezes que o algoritmo executará
	const int numeros_organizados = i_final - i_inicio;

	for (int j = 0; j < numeros_organizados; j++) {
		for (int i = i_inicio; i < i_final - j; i++) {
			if (i + 1 == i_final - j) {
				// está no ultimo número da sub-lista
				break;
			}

			// se o item atual for nulo, ele passa adiante
			// trocando de lugar com o próximo valor
			if (compararItem(&mochila[i], &ItemNulo)) {
				// como o valor é nulo, não há necessidade de armazenar seu valor
				mochila[i] = mochila[i + 1];
				mochila[i + 1] = ItemNulo;
			}
		}
	}
}

void removerItem() {
	// para evitar o uso de memória desnecessário,
	// verifico primeiro se a mochila não está vazia e
	// só depois definir variáveis/dados
	if (mochila == NULL) {
		return;
	}

	// por se tratar de um "unsigned int", ele não pode ser negativo
	if (mochila_quantidade == 0) {
		printf("[!] A mochila está vazia!\n");
		return;
	}

	// variável para armazenar os dados informados pelo usuário
	char nome[MAX_STRING];

	printf("--------[Remover Item]--------\n");

	// eu utilizo o " %24[^\n]" para ler até 24 caracteres (mais o caráctere \0, para terminação de string)
	// e permitindo textos com espaço, terminando a linha no Enter (\n)
	printf("\nInsira o nome do item:\n>> ");
	scanf(" %24[^\n]", &nome);

	printf("[*] Procurando por: \"%s\"...\n", nome);


	// variável para armazenar índice onde foi localizado o item
	int indice_achado = -1;

	// realiza um for loop para procurar por toda a mochila:
	for (int i = 0; i < MAX_ITEMS; i++) {
		// se o item for nulo, ele passa para o próximo
		// evitando checagens desnecessárias
		if (compararItem(&mochila[i], &ItemNulo)) {
			continue;
		}

		// use "strcmp" para comparar o nome do item
		if (strcmp(mochila[i].nome, nome) == 0) {
			// se forem iguais (strcmp == 0),
			// ele salva o indice e para a busca
			indice_achado = i;
			break;
		}
	}

	// encerra a função caso não encontre o item
	if (indice_achado == -1) {
		printf("[!] O item \"%s\" não foi encontrado na mochila!\n", nome);
		return;
	}

	// variável para armazenar a opção selecionada
	unsigned int escolha = 0;
	do {
		limparTela();

		printf("------------------------------\n");
		printf("[*] Item \"%s\" encontrado!\n", nome);
		printf("[>] Índice: %d/%d\n", indice_achado + 1, mochila_quantidade);
		printf("------------------------------\n");
		printf("[*] Você tem certeza que\n");
		printf("    deseja remover este item?\n");
		printf("------------------------------\n");
		printf(" [1] - Remover item\n");
		printf(" [0] - Cancelar\n");
		printf("------------------------------\n");

		printf(">> ");
		scanf("%d", &escolha);
		printf("\n");

		switch (escolha) {
		case 0:
			printf("[*] Cancelando...\n");
			return;

		case 1:
			printf("[*] Removendo item...\n");
			break;

		default:
			printf("[!] Opção inválida\n");
			break;
		}
	} while (escolha != 1);


	mochila[indice_achado] = ItemNulo;

	// verifica se a remoção falhou
	if (!compararItem(&mochila[indice_achado], &ItemNulo)) {
		printf("[!] Falha ao tentar remover o item\n");
		return;
	}

	printf("[*] Item removido com sucesso!\n");

	// reorganiza a mochila:
	// começando do ponto que houve uma alteração
	// finalizando ao ponto marcado pela variável mochila_quantidade
	organizar_mochila(indice_achado, mochila_quantidade);

	// depois de reorganizar a mochila, atualiza a quantidade de itens dela
	mochila_quantidade--;
}

void listarItens() {
	// se a mochila não tiver sido alocada ele retorna
	if (mochila == NULL) {
		return;
	}

	printf("[*] Listando itens na mochila...\n");

	if (mochila_quantidade == 0) {
		printf("[!] A mochila está vazia!\n");
		return;
	}

	// realiza um for loop para testar/listar todos os itens da mochila
	for (int i = 0; i < mochila_quantidade; i++) {
		// c_item: item atual do laço
		const Item* c_item = &mochila[i];

		// se o item atual não for nulo
		if (!compararItem(&mochila[i], &ItemNulo)) {
			printf("[>] (Item %d/%d) ", i + 1, mochila_quantidade);
			imprimirItem(c_item, false);
		}
	}

	printf("[*] Os itens da mochila foram listados!\n");
}

void procurarItem() {
	// para evitar o uso de memória desnecessário,
	// verifico primeiro se a mochila não está vazia e
	// só depois definir variáveis/dados
	if (mochila == NULL) {
		return;
	}

	// por se tratar de um "unsigned int", ele não pode ser negativo
	if (mochila_quantidade == 0) {
		printf("[!] A mochila está vazia!\n");
		return;
	}

	// variável para armazenar os dados informados pelo usuário
	char nome[MAX_STRING];

	printf("--------[Buscar Itens]--------\n");

	// eu utilizo o " %24[^\n]" para ler até 24 caracteres (mais o caráctere \0, para terminação de string)
	// e permitindo textos com espaço, terminando a linha no Enter (\n)
	printf("\nInsira o nome do item:\n>> ");
	scanf(" %24[^\n]", &nome);

	printf("[*] Procurando por: \"%s\"...\n", nome);


	// variável para armazenar índice onde foi localizado o item
	int indice_achado = -1;

	// realiza um for loop para procurar por toda a mochila:
	for (int i = 0; i < MAX_ITEMS; i++) {
		// se o item for nulo, ele passa para o próximo
		// evitando checagens desnecessárias
		if (compararItem(&mochila[i], &ItemNulo)) {
			continue;
		}

		// use "strcmp" para comparar o nome do item
		if (strcmp(mochila[i].nome, nome) == 0) {
			// se forem iguais (strcmp == 0),
			// ele salva o indice e para a busca
			indice_achado = i;
			break;
		}
	}

	// encerra a função caso não encontre o item
	if (indice_achado == -1) {
		printf("[!] O item \"%s\" não foi encontrado na mochila!\n", nome);
		return;
	}

	limparTela();
	printf("------------------------------\n");
	printf("[*] Item encontrado!\n");
	printf("[>] Índice: %d/%d\n", indice_achado + 1, mochila_quantidade);
	printf("------------------------------\n");
	imprimirItem(&mochila[indice_achado], true);
}

void exibirMenu() {
	// variável para armazenar a opção selecionada pelo usuário
	unsigned int escolha = 0;
	do {
		// limpa a tela sempre, antes de (re)imprimir o menu
		printf("-------[Menu Principal]-------\n");
		printf(" > Selecione uma ação:\n");
		printf(" - [1]: Inserir item\n");
		printf(" - [2]: Remover item\n");
		printf(" - [3]: Listar mochila\n");
		printf(" - [4]: Procurar item\n");
		printf(" - [0]: Encerra o jogo\n");
		printf("------------------------------\n");

		printf(">> ");
		scanf("%d", &escolha);
		printf("\n");

		limparTela();
		switch (escolha) {
		case 0:
			printf("[*] Saindo do jogo...\n");
			return;

		case 1:
			printf("[*] Abrindo modo de inserção...\n");
			inserirItem();
			break;

		case 2:
			printf("[*] Abrindo modo de remoção...\n");
			removerItem();
			break;

		case 3:
			printf("[*] Abrindo modo de listagem...\n");
			listarItens();
			break;

		case 4:
			printf("[*] Abrindo modo de busca...\n");
			procurarItem();
			break;

		default:
			printf("[!] Opção inválida\n");
			break;
		}
	} while (true);
}

int main() {

	printf("[*] Alocando mochila na memória...\n");
	if (!alocarMochila()) {
		printf("[X] Falha ao alocar o vetor mochila na memória\n");
		return -1;
	}

	exibirMenu();

	limparMemoriaMochila();

	return 0;
}
