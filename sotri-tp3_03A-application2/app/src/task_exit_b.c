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
#define G_TASK_EXIT_B_CNT_INI	0ul

#define TASK_EXIT_B_DEL_ZERO	(pdMS_TO_TICKS(0ul))
#define TASK_EXIT_B_DEL_MAX		(pdMS_TO_TICKS(2500ul))

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_exit_b_wait_2500mS		= "   ==> Task Exit B  - Wait:   2500mS";

/********************** external data declaration *****************************/
uint32_t g_task_exit_b_cnt;

/********************** external functions definition ************************/
/* Task thread */
void task_exit_b(void *parameters)
{
    g_task_exit_b_cnt = 0ul;
    LOGGER_INFO("  %s is running", pcTaskGetName(NULL));

    for (;;)
    {
        xSemaphoreTake(h_exit_b_bin_sem, portMAX_DELAY);
        g_task_exit_b_cnt++;

        xSemaphoreTake(h_mutex_mut_sem, portMAX_DELAY);
        {
            if (g_tasks_cnt > 0)
            {
                g_tasks_cnt--;
                LOGGER_INFO("Vehiculo saliendo de B. Total en cruce: %lu", g_tasks_cnt);

                if (g_tasks_cnt == 0)
                {
                    semaforo_a = 1;
                    semaforo_b = 1;
                    LOGGER_INFO("Cruce vacio de forma segura. AMBOS semaforos en VERDE.");
                    xSemaphoreGive(h_go_a_bin_sem);
                    xSemaphoreGive(h_go_b_bin_sem);
                }
                else if (g_tasks_cnt < G_TASKS_CNT_MAX)
                {
                    semaforo_b = 1;
                    LOGGER_INFO("Espacio liberado en el cruce. Semaforo B habilitado.");
                    xSemaphoreGive(h_go_b_bin_sem);
                }
            }
        }
        xSemaphoreGive(h_mutex_mut_sem);
    }


}

/********************** end of file ******************************************/
