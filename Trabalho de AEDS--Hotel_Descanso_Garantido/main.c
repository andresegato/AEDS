  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <time.h>
  //#include "munit.h" a biblioteca nao esta funcionando...

  #define MAX_CLIENTES 100
  #define MAX_FUNCIONARIOS 100
  #define MAX_ESTADIAS 100
  #define MAX_QUARTOS 50
  #define VALOR_DIARIA 500.0

  // Definir estruturas e variáveis globais

  typedef struct {
      int codigo;
      char nome[100];
      char endereco[100];
      char telefone[20];
      int codigo_funcionario;
  } Cliente;

  typedef struct {
      int codigo;
      char nome[50];
      char cargo[50];
      float salario;
  } Funcionario;

  typedef struct {
      int dia;
      int mes;
      int ano;
  } Data;

  typedef struct {
      Data entrada;
      Data saida;
      int codigo_cliente;
      int codigo_funcionario;
      int quantidade_hospedes;
      int numero_quarto;
      float valor_total;
  } Estadia;

  Cliente clientes[MAX_CLIENTES];
  int countClientes = 0;

  Funcionario funcionarios[MAX_FUNCIONARIOS];
  int countFuncionarios = 0;

  Estadia estadias[MAX_ESTADIAS];
  int countEstadias = 0;

  // Protótipos das funções
  void cadastrar_cliente();
  void cadastrar_estadia();
  void cadastrar_funcionario();
  void listar_clientes();
  void listar_estadias();
  void listar_funcionarios();
  int verificar_disponibilidade_quarto(int quantidade_hospedes, int *numero_quarto);
  int dataValida(int dia, int mes, int ano);
  int calcular_dias(Data entrada, Data saida);
  float calcular_valor_estadia(Data entrada, Data saida, float valor_diaria);
  int gerar_codigo_cliente();
  int gerar_codigo_funcionario();
  void salvar_funcionarios_txt();
  void carregar_funcionarios();
  void salvar_clientes_txt();
  void carregar_clientes();
  void mostrarMensagemSeVazio(int quantidade, const char *mensagem);

  // Implementação das funções

  void mostrarMensagemSeVazio(int quantidade, const char *mensagem) {
      if (quantidade == 0) {
          printf("%s\n", mensagem);
      }
  }

  int dataValida(int dia, int mes, int ano) {
      if (ano < 2024) return 0;
      if (mes < 1 || mes > 12) return 0;
      if (dia < 1 || dia > 31) return 0;
      return 1;
  }

  int calcular_dias(Data entrada, Data saida) {
      struct tm entrada_tm = {0, 0, 14, entrada.dia, entrada.mes - 1, entrada.ano - 1900};
      struct tm saida_tm = {0, 0, 12, saida.dia, saida.mes - 1, saida.ano - 1900};
      time_t tempoEntrada = mktime(&entrada_tm);
      time_t tempoSaida = mktime(&saida_tm);
      return difftime(tempoSaida, tempoEntrada) / (60 * 60 * 24);
  }

  float calcular_valor_estadia(Data entrada, Data saida, float valor_diaria) {
      int dias = calcular_dias(entrada, saida);
      float valor_total = dias * valor_diaria;
      return valor_total;
  }

  int verificar_disponibilidade_quarto(int quantidade_hospedes, int *numero_quarto) {
      *numero_quarto = rand() % MAX_QUARTOS + 1;
      return 1;
  }

  int gerar_codigo_cliente() {
      return rand() % 10000 + 1; // Gerar um código aleatório entre 1 e 10000
  }

  int gerar_codigo_funcionario() {
      return rand() % 1000 + 1; // Gerar um código aleatório entre 1 e 1000
  }

  void cadastrar_cliente() {
      if (countClientes >= MAX_CLIENTES) {
          printf("Limite máximo de clientes atingido.\n");
          return;
      }

      getchar();
      printf("Digite o nome completo do cliente: ");
      fgets(clientes[countClientes].nome, sizeof(clientes[countClientes].nome), stdin);
      clientes[countClientes].nome[strcspn(clientes[countClientes].nome, "\n")] = '\0';

      printf("Digite o endereço do cliente: ");
      fgets(clientes[countClientes].endereco, sizeof(clientes[countClientes].endereco), stdin);
      clientes[countClientes].endereco[strcspn(clientes[countClientes].endereco, "\n")] = '\0';

      while (1) {
          printf("Digite o telefone (apenas 9 dígitos) do cliente: ");
          fgets(clientes[countClientes].telefone, sizeof(clientes[countClientes].telefone), stdin);
          clientes[countClientes].telefone[strcspn(clientes[countClientes].telefone, "\n")] = '\0';

          if (strlen(clientes[countClientes].telefone) != 9) {
              printf("Telefone inválido. Deve conter exatamente nove dígitos.\n");
          } else {
              break;
          }
      }

      clientes[countClientes].codigo = gerar_codigo_cliente();
      clientes[countClientes].codigo_funcionario = gerar_codigo_funcionario(); // Atribuir um funcionário responsável (simulação)

      printf("Cliente cadastrado com sucesso. Código do cliente: %d\n", clientes[countClientes].codigo);
      countClientes++;

      salvar_clientes_txt(); // Salvar clientes no arquivo após cada cadastro
  }

  void cadastrar_estadia() {
      int codigo_cliente;
      int quantidade_hospedes;
      Data entrada, saida;

      printf("\n=== Cadastro de Estadia ===\n");
      printf("\nValor da Diária: R$500.00\n");

      if (countClientes == 0) {
          printf("\nNão há clientes cadastrados. Cadastre um cliente primeiro.\n");
          return;
      }

      printf("\nDigite o código do cliente que deseja se hospedar: ");
      scanf("%d", &codigo_cliente);
      getchar();

      Cliente *cliente = NULL;
      for (int i = 0; i < countClientes; i++) {
          if (clientes[i].codigo == codigo_cliente) {
              cliente = &clientes[i];
              break;
          }
      }

      if (cliente == NULL) {
          printf("Cliente não encontrado. Verifique o código e tente novamente.\n");
          return;
      }

      printf("Digite a quantidade de hóspedes: ");
      scanf("%d", &quantidade_hospedes);
      getchar();

      while (1) {
          printf("Digite a data de entrada desejada (DD/MM/AAAA): ");
          scanf("%d/%d/%d", &entrada.dia, &entrada.mes, &entrada.ano);
          getchar();

          if (!dataValida(entrada.dia, entrada.mes, entrada.ano)) {
              printf("Data de entrada inválida. Digite novamente.\n");
              continue;
          }

          printf("Digite a data de saída desejada (DD/MM/AAAA): ");
          scanf("%d/%d/%d", &saida.dia, &saida.mes, &saida.ano);
          getchar();

          if (!dataValida(saida.dia, saida.mes, saida.ano)) {
              printf("Data de saída inválida. Digite novamente.\n");
              continue;
          }

          if (saida.ano < entrada.ano ||
              (saida.ano == entrada.ano && saida.mes < entrada.mes) ||
              (saida.ano == entrada.ano && saida.mes == entrada.mes && saida.dia <= entrada.dia)) {
              printf("Erro: Data de saída deve ser posterior à data de entrada. Digite novamente.\n");
              continue;
          }

          int numero_quarto;
          if (!verificar_disponibilidade_quarto(quantidade_hospedes, &numero_quarto)) {
              printf("Quarto não disponível para as datas selecionadas. Escolha outras datas.\n");
              continue;
          }

          float valor_total_estadia = calcular_valor_estadia(entrada, saida, VALOR_DIARIA);

          estadias[countEstadias].entrada = entrada;
          estadias[countEstadias].saida = saida;
          estadias[countEstadias].codigo_cliente = cliente->codigo;
          estadias[countEstadias].codigo_funcionario = cliente->codigo_funcionario;
          estadias[countEstadias].quantidade_hospedes = quantidade_hospedes;
          estadias[countEstadias].numero_quarto = numero_quarto;
          estadias[countEstadias].valor_total = valor_total_estadia;

          countEstadias++;

          printf("Estadia cadastrada com sucesso.\n");
          break;
      }
  }

void cadastrar_funcionario() {
    while (1) {
        if (countFuncionarios >= MAX_FUNCIONARIOS) {
            printf("Limite máximo de funcionários atingido.\n");
            return;
        }

        getchar();
        printf("Digite o nome completo do funcionário: ");
        fgets(funcionarios[countFuncionarios].nome, sizeof(funcionarios[countFuncionarios].nome), stdin);
        funcionarios[countFuncionarios].nome[strcspn(funcionarios[countFuncionarios].nome, "\n")] = '\0';

        char cargo[50];
        int cargo_valido = 0;
        while (!cargo_valido) {
            printf("Digite o cargo do funcionário (recepcionista, auxiliar de limpeza, garçom, gerente): ");
            fgets(cargo, sizeof(cargo), stdin);
            cargo[strcspn(cargo, "\n")] = '\0';

            // Verifica se o cargo é válido
            if (strcmp(cargo, "recepcionista") == 0 ||
                strcmp(cargo, "auxiliar de limpeza") == 0 ||
                strcmp(cargo, "garçom") == 0 ||
                strcmp(cargo, "gerente") == 0) {
                cargo_valido = 1;
            } else {
                printf("Cargo inválido. Escolha entre recepcionista, auxiliar de limpeza, garçom ou gerente.\n");
            }
        }

        strcpy(funcionarios[countFuncionarios].cargo, cargo);

        printf("Digite o salário do funcionário: ");
        scanf("%f", &funcionarios[countFuncionarios].salario);
        getchar();

        funcionarios[countFuncionarios].codigo = gerar_codigo_funcionario();

        printf("Funcionário cadastrado com sucesso. Código do funcionário: %d\n", funcionarios[countFuncionarios].codigo);
        countFuncionarios++;

        salvar_funcionarios_txt(); // Salvar funcionários no arquivo após cada cadastro

        char opcao;
        printf("Deseja cadastrar outro funcionário? (s/n): ");
        scanf(" %c", &opcao);
        getchar();

        if (opcao != 's' && opcao != 'S') {
            break; // Sai do loop se a opção não for 's' ou 'S'
        }
    }
}

  void listar_clientes() {
      printf("\n=== Lista de Clientes ===\n");
      mostrarMensagemSeVazio(countClientes, "\nNão há clientes cadastrados.");

      for (int i = 0; i < countClientes; i++) {
          printf("\nCódigo: %d\n", clientes[i].codigo);
          printf("Nome: %s\n", clientes[i].nome);
          printf("Endereço: %s\n", clientes[i].endereco);
          printf("Telefone: %s\n", clientes[i].telefone);
          printf("Código Funcionário: %d\n", clientes[i].codigo_funcionario);
          printf("-------------------\n");
      }
  }

  void listar_estadias() {
      printf("\n=== Lista de Estadias ===\n");
      mostrarMensagemSeVazio(countEstadias, "\nNão há estadias cadastradas.");

      for (int i = 0; i < countEstadias; i++) {
          printf("\nCódigo Cliente: %d\n", estadias[i].codigo_cliente);
          printf("Código Funcionário: %d\n", estadias[i].codigo_funcionario);
          printf("Data de Entrada: %02d/%02d/%04d\n", estadias[i].entrada.dia, estadias[i].entrada.mes, estadias[i].entrada.ano);
          printf("Data de Saída: %02d/%02d/%04d\n", estadias[i].saida.dia, estadias[i].saida.mes, estadias[i].saida.ano);
          printf("Número do Quarto: %d\n", estadias[i].numero_quarto);
          printf("Quantidade de Hóspedes: %d\n", estadias[i].quantidade_hospedes);
          printf("Valor Total: R$ %.2f\n", estadias[i].valor_total);
          printf("-------------------\n");
      }
  }

  void listar_funcionarios() {
      printf("\n=== Lista de Funcionários ===\n");
      mostrarMensagemSeVazio(countFuncionarios, "\nNão há funcionários cadastrados.\n");

      for (int i = 0; i < countFuncionarios; i++) {
          printf("\nCódigo: %d\n", funcionarios[i].codigo);
          printf("Nome: %s\n", funcionarios[i].nome);
          printf("Cargo: %s\n", funcionarios[i].cargo);
          printf("Salário: R$ %.2f\n", funcionarios[i].salario);
          printf("-------------------\n");
      }
  }

  void salvar_clientes_txt() {
      FILE *arquivo = fopen("clientes.txt", "w");
      if (arquivo == NULL) {
          perror("Erro ao abrir o arquivo clientes.txt");
          return;
      }

      for (int i = 0; i < countClientes; i++) {
          fprintf(arquivo, "%d;%s;%s;%s;%d\n", clientes[i].codigo, clientes[i].nome, clientes[i].endereco, clientes[i].telefone, clientes[i].codigo_funcionario);
      }

      fclose(arquivo);
  }

  void salvar_funcionarios_txt() {
      FILE *arquivo = fopen("funcionarios.txt", "w");
      if (arquivo == NULL) {
          perror("Erro ao abrir o arquivo funcionarios.txt");
          return;
      }

      for (int i = 0; i < countFuncionarios; i++) {
          fprintf(arquivo, "%d;%s;%s;%.2f\n", funcionarios[i].codigo, funcionarios[i].nome, funcionarios[i].cargo, funcionarios[i].salario);
      }

      fclose(arquivo);
  }

  void carregar_clientes() {
      FILE *arquivo = fopen("clientes.txt", "r");
      if (arquivo == NULL) {
          perror("Erro ao abrir o arquivo clientes.txt");
          return;
      }

      while (fscanf(arquivo, "%d;%[^;];%[^;];%[^;];%d\n", &clientes[countClientes].codigo, clientes[countClientes].nome, clientes[countClientes].endereco, clientes[countClientes].telefone, &clientes[countClientes].codigo_funcionario) == 5) {
          countClientes++;
      }

      fclose(arquivo);
  }

  void carregar_funcionarios() {
      FILE *arquivo = fopen("funcionarios.txt", "r");
      if (arquivo == NULL) {
          perror("Erro ao abrir o arquivo funcionarios.txt");
          return;
      }

      while (fscanf(arquivo, "%d;%[^;];%[^;];%f\n", &funcionarios[countFuncionarios].codigo, funcionarios[countFuncionarios].nome, funcionarios[countFuncionarios].cargo, &funcionarios[countFuncionarios].salario) == 4) {
          countFuncionarios++;
      }

      fclose(arquivo);
  }

  int main() {
      srand(time(NULL)); // Inicializar gerador de números aleatórios com o tempo atual

      carregar_clientes();
      carregar_funcionarios();

      int opcao;
      do {
          printf("\nSeja Bem Vindo ao Hotel Descanso Garantido!\n");
          printf("\n=== Menu Principal ===\n");
          printf("\n1 - Cadastrar Cliente\n");
          printf("2 - Cadastrar Estadia\n");
          printf("3 - Cadastrar Funcionário\n");
          printf("4 - Listar Clientes\n");
          printf("5 - Listar Estadias\n");
          printf("6 - Listar Funcionários\n");
          printf("7 - Sair\n");
          printf("\nEscolha uma opção: ");
          scanf("%d", &opcao);
          getchar(); // Limpar o buffer de entrada

          switch (opcao) {
              case 1:
                  cadastrar_cliente();
                  break;
              case 2:
                  cadastrar_estadia();
                  break;
              case 3:
                  cadastrar_funcionario();
                  break;
              case 4:
                  listar_clientes();
                  break;
              case 5:
                  listar_estadias();
                  break;
              case 6:
                  listar_funcionarios();
                  break;
              case 7:
                  printf("Encerrando o programa...\n");
                  break;
              default:
                  printf("Opção inválida. Tente novamente.\n");
          }
      } while (opcao != 7);

      return 0;
  }
