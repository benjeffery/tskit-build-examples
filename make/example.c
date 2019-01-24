#include <stdio.h>
#include <stdlib.h>
#include <tskit.h>
#include <err.h>

/* Use the tskit tables API to build a simple tree:
 *     4        time=2
 *   +-++
 *   |  |
 *   |  3       time=1
 *   | +++
 *   | | |
 *   2 0 1      time=0
 *
 */

#define check_tsk_error(val) if (val < 0) {\
    fprintf(stderr, "line %d: %s\n", __LINE__, tsk_strerror(val));\
    exit(EXIT_FAILURE);\
}

int main()
{
    int ret;
    tsk_table_collection_t tables;

    ret = tsk_table_collection_init(&tables, 0);
    check_tsk_error(ret);

    tables.sequence_length = 1.0;
    /* Add the samples */
    ret = tsk_node_table_add_row(&tables.nodes, TSK_NODE_IS_SAMPLE,
            0, TSK_NULL, TSK_NULL, NULL, 0);
    check_tsk_error(ret);
    ret = tsk_node_table_add_row(&tables.nodes, TSK_NODE_IS_SAMPLE,
            0, TSK_NULL, TSK_NULL, NULL, 0);
    check_tsk_error(ret);
    ret = tsk_node_table_add_row(&tables.nodes, TSK_NODE_IS_SAMPLE,
            0, TSK_NULL, TSK_NULL, NULL, 0);
    check_tsk_error(ret);
    /* Add internal nodes */
    ret = tsk_node_table_add_row(&tables.nodes, TSK_NODE_IS_SAMPLE,
            1, TSK_NULL, TSK_NULL, NULL, 0);
    check_tsk_error(ret);
    ret = tsk_node_table_add_row(&tables.nodes, TSK_NODE_IS_SAMPLE,
            2, TSK_NULL, TSK_NULL, NULL, 0);
    check_tsk_error(ret);

    /* Add edges */
    ret = tsk_edge_table_add_row(&tables.edges, 0, 1, 3, 0);
    check_tsk_error(ret);
    ret = tsk_edge_table_add_row(&tables.edges, 0, 1, 4, 3);
    check_tsk_error(ret);
    ret = tsk_edge_table_add_row(&tables.edges, 0, 1, 3, 1);
    check_tsk_error(ret);
    ret = tsk_edge_table_add_row(&tables.edges, 0, 1, 4, 2);

    /* Edges must be sorted in a specific order for efficiency. Sort the
     * tables to make sure that this requirement is fulfilled. */
    ret = tsk_table_collection_sort(&tables, NULL, 0);
    check_tsk_error(ret);

    /* Write out to file */
    ret = tsk_table_collection_dump(&tables, "example.trees", 0);
    check_tsk_error(ret);

    tsk_table_collection_free(&tables);
    return 0;
}
