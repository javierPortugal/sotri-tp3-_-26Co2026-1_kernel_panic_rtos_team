/*
 * Copyright (c) 2026 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"
#include "cmsis_os.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"

/********************** macros and definitions *******************************/
#define G_TASK_EXIT_A_CNT_INI	0ul

#define TASK_EXIT_A_DEL_ZERO	(pdMS_TO_TICKS(0ul))
#define TASK_EXIT_A_DEL_MAX		(pdMS_TO_TICKS(2500ul))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_exit_a_wait_2500mS		= "   ==> Task Exit A  - Wait:   2500mS";

/********************** external data declaration *****************************/
uint32_t g_task_exit_a_cnt;

/********************** external functions definition ************************/
/* Task thread */
void task_exit_a(void *parameters)
{
    /* Declarar e inicializar variables de la función de la tarea */
    g_task_exit_a_cnt = G_TASK_EXIT_A_CNT_INI;

    LOGGER_INFO(" ");
    LOGGER_INFO("  %s is running - Tick [mS] = %lu", pcTaskGetName(NULL), xTaskGetTickCount());

    for (;;)
    {
        // 1. Bloqueo eficiente: Esperar la señal física del sensor de salida A
        xSemaphoreTake(h_exit_a_bin_sem, portMAX_DELAY);

        /* Update Task Counter */
        g_task_exit_a_cnt++;

        // 2. Proteger el acceso al contador y semáforos globales bajo el Mutex
        xSemaphoreTake(h_mutex_mut_sem, portMAX_DELAY);
        {
            if (g_tasks_cnt > 0)
            {
                g_tasks_cnt--;
                LOGGER_INFO("Vehiculo saliendo de A. Total en cruce: %lu", g_tasks_cnt);

                // CONTROL DE ALTERNANCIA Y DESBLOQUEO
                if (g_tasks_cnt == 0)
                {
                    // CORRECCIÓN CRÍTICA: Si el cruce se vacía, AMBOS vuelven a estar disponibles.
                    // El primer vehículo que llegue de cualquier lado tomará la prioridad.
                    semaforo_a = 1;
                    semaforo_b = 1;
                    LOGGER_INFO("Cruce vacio de forma segura. AMBOS semaforos en VERDE.");

                    // Despertamos ambas entradas de control por si había autos en espera
                    xSemaphoreGive(h_go_a_bin_sem);
                    xSemaphoreGive(h_go_b_bin_sem);
                }
                else if (g_tasks_cnt < G_TASKS_CNT_MAX)
                {
                    // Si aún quedan autos en el cruce pero se liberó un lugar,
                    // permitimos que la ráfaga de la entrada A continúe fluyendo.
                    semaforo_a = 1;
                    LOGGER_INFO("Espacio liberado en el cruce. Semaforo A habilitado.");

                    // Despertamos específicamente a la entrada A para evaluar el paso
                    xSemaphoreGive(h_go_a_bin_sem);
                }
            }
        }
        xSemaphoreGive(h_mutex_mut_sem);
    }
}

/********************** end of file ******************************************/
