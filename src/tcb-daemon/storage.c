/* src/tcb-daemon/storage.c */
#include "tcb.h"

static sqlite3 *db = NULL;

void storage_init(void) {
    char db_path[512];
    const char *home = getenv("HOME");
    
    if (home) {
        snprintf(db_path, sizeof(db_path), "%s/.tcb.db", home);
    } else {
        snprintf(db_path, sizeof(db_path), "./.tcb.db");
    }

    int rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Cannot open SQLite database: %s", sqlite3_errmsg(db));
        return;
    }

    const char *sql_create_table = 
        "CREATE TABLE IF NOT EXISTS clipboard_history ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "hash TEXT UNIQUE NOT NULL,"
        "content TEXT NOT NULL,"
        "length INTEGER NOT NULL,"
        "timestamp INTEGER NOT NULL"
        ");"
        "CREATE INDEX IF NOT EXISTS idx_timestamp ON clipboard_history(timestamp DESC);";

    char *err_msg = NULL;
    rc = sqlite3_exec(db, sql_create_table, NULL, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("SQL error creating structural tables: %s", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    LOG_INFO("Successfully initialized SQL persistence engine at %s", db_path);
}

void storage_push(const char *data, size_t len) {
    if (len == 0 || len > MAX_PAYLOAD_SIZE || !db) return;

    uint64_t now = (uint64_t)time(NULL);
    
    char hash_str[17];
    uint64_t hash = calculate_fnv1a(data, len);
    sprintf(hash_str, "%016llx", (unsigned long long)hash);

    const char *sql_insert = 
        "INSERT INTO clipboard_history (hash, content, length, timestamp) "
        "VALUES (?, ?, ?, ?) "
        "ON CONFLICT(hash) DO UPDATE SET timestamp = excluded.timestamp;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to prepare insertion statement: %s", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, hash_str, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, data, len, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, (int)len);
    sqlite3_bind_int64(stmt, 4, (int64_t)now);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        LOG_ERROR("Failed to execute database write transaction: %s", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);

    char sql_cleanup[128];
    sprintf(sql_cleanup, 
            "DELETE FROM clipboard_history WHERE id NOT IN "
            "(SELECT id FROM clipboard_history ORDER BY timestamp DESC LIMIT %d);", 
            STORAGE_MAX_ENTRIES);
    
    sqlite3_exec(db, sql_cleanup, NULL, 0, NULL);
}

void storage_get_all(clip_entry_t *out_buffer, size_t *out_count) {
    *out_count = 0;
    if (!db) return;

    const char *sql_select = "SELECT content, length, timestamp FROM clipboard_history ORDER BY timestamp DESC;";
    
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to prepare retrieval query: %s", sqlite3_errmsg(db));
        return;
    }

    size_t idx = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && idx < STORAGE_MAX_ENTRIES) {
        const char *content = (const char *)sqlite3_column_text(stmt, 0);
        int length = sqlite3_column_int(stmt, 1);
        uint64_t timestamp = (uint64_t)sqlite3_column_int64(stmt, 2);

        out_buffer[idx].data = malloc(length + 1);
        if (out_buffer[idx].data) {
            memcpy(out_buffer[idx].data, content, length);
            out_buffer[idx].data[length] = '\0';
            out_buffer[idx].length = length;
            out_buffer[idx].timestamp = timestamp;
            idx++;
        }
    }

    *out_count = idx;
    sqlite3_finalize(stmt);
}

void storage_free(void) {
    if (db) {
        sqlite3_close(db);
        db = NULL;
    }
}