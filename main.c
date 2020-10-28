#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


#define PROD_AMOUNT 800
pthread_mutex_t lock;

int i = 0, prodCount = 0, prodWeights[PROD_AMOUNT], randomNumber;

int generateSensorValue ()
{
        randomNumber = rand() + 1;
        if(randomNumber%2 == 0)
        {
                return 1;
        }
        else
        {
                return 0;
        }
}

void* sumProducts(void * arg)
{
        while(prodCount < PROD_AMOUNT)
        {
                if (generateSensorValue() == 1)
                {
                        if(prodCount < PROD_AMOUNT)
                        {
                                pthread_mutex_lock(&lock);
                                prodCount++;
                                prodWeights[i] = 1;
                                i++;
                                pthread_mutex_unlock(&lock);
                                printf("I am locked with: %lu.\n", pthread_self());
                                printf("Valor de I: %d\n",i);
                        }
                }

        }
        return NULL;
}

int main()
{
                
                
        float maintime_time = clock();
        srand(time(NULL));
        int a, prodWeightsSUM = 0, continue_ex = 1, j, c;

        do
        {
                pthread_t threadMat1, threadMat2, threadMat3;
                pthread_attr_t attr;
                pthread_mutexattr_t mutexAttrPrioInherit;
                int mutexProtocol;

                pthread_mutexattr_init(&mutexAttrPrioInherit);
                pthread_mutexattr_getprotocol(&mutexAttrPrioInherit, &mutexProtocol);
                if (mutexProtocol != PTHREAD_PRIO_INHERIT)
                {
                        pthread_mutexattr_setprotocol(&mutexAttrPrioInherit, PTHREAD_PRIO_INHERIT);
                        printf("No property heritage found.\n"); // apaga depois
                }

                if(pthread_mutex_init(&lock, NULL))
                {
                        printf("ERROR!!!!!!");
                        return 6;
                }
                pthread_attr_init(&attr);
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

                //while(prodCount < PROD_AMOUNT)
                //{
                if(pthread_create(&threadMat1, &attr, sumProducts, NULL))
                {
                        fprintf(stderr, "ERROR - Thread 1 creation.\n");
                        return 1;
                }
                if(pthread_create(&threadMat2, &attr, sumProducts, NULL))
                {
                        fprintf(stderr, "ERROR - Thread 2 creation.\n");
                        return 2;
                }

                if(pthread_create(&threadMat3, &attr, sumProducts, NULL))
                {
                        fprintf(stderr, "ERROR - Thread 3 creation.\n");
                        return 3;
                }

                pthread_attr_destroy(&attr);
                if(pthread_join(threadMat1,NULL))
                {
                        fprintf(stderr, "ERROR - Thread 1 join.\n");
                        return 4;
                }

                if(pthread_join(threadMat2,NULL))
                {
                        fprintf(stderr, "ERROR - Thread 2 join.\n");
                        return 5;
                }

                if(pthread_join(threadMat3, NULL))
                {
                        fprintf(stderr, "ERROR - Thread 3 join\n");
                        return 6;
                }
        //}


                printf("Products Gone Through Mat: %d\n", prodCount);

                //iniciar o timer
                for(a = 0; a < PROD_AMOUNT; a++)
                {
                        prodWeightsSUM = prodWeightsSUM + prodWeights[a];
                }
                // finalizar o timer
                printf("Tempo de execucao: %d\n", (clock() - maintime_time) );
                printf("Exit? (1 YES , 0 NO).\n");
                scanf("%d", &continue_ex);
                while((c = getchar()) != '\n' && c != EOF){}

                if(continue_ex == 1)
                {
                        // Cleaning Runtime Variables
                        prodCount = 0;
                        i = 0;
                        pthread_mutex_destroy(&lock);
                        for(a = 0; a < PROD_AMOUNT; a++)
                        {
                                prodWeights[a] = 0;
                        }
                }

                printf("Valor do J: %d\n",j);
                j++;
        }while(continue_ex == 0);

        pthread_exit(NULL);

        return 0;
}