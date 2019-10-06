#pragma once

#include <string>

namespace d_common {
class application_files {
//public:
//    static constexpr char KEY_STORE_APPENDIX_ENCODER[] = "encodr";
//#ifdef IS_THIS_UNIT_TEST
//    static constexpr const char APP_NAME[] = "MusicThingyCpp/tests";
//    static constexpr const char REMOTE_DB_ON_DROPBOX_FILENAME[] = "/my_test_items.bin";
//    static constexpr const char LEGACY_DB_ON_DROPBOX_FILENAME[] = "/legacy_db_tests.bin";
//    static constexpr const char LEGACY_DB_LOCAL_COPY_FILENAME[] = "lDbWrkCpy_tests.bin";
//    static constexpr char DROPBOX_KEY_DESC[] = "MusicThingyCppDropboxKey__tests";
//    /**
//     * String that describes a key which is used for encoding and decoding data that gets
//     * transfered over public web.
//     */
//    static constexpr char SSL_ENCODE_DB_DESC[] = "MusicThingyCppDbSecret__tests";
//    static constexpr char SSL_ITEM_ENCODING_KEY_DESC[] =
//        "MusicThingyCppBinaryEncodingKey__tests";
//#elif defined(MUSIC_SOFTWARE_EXAMPLES)
//    static constexpr const char APP_NAME[] = "MusicThingyCpp/examples";
//    static constexpr const char REMOTE_DB_ON_DROPBOX_FILENAME[] = "/my_x_items.bin";
//    static constexpr char DROPBOX_KEY_DESC[] = "MusicThingyCppDropboxKey__examples";
//    static constexpr char SSL_ENCODE_DB_DESC[] = "MusicThingyCppDbSecret__examples";
//    static constexpr char SSL_ITEM_ENCODING_KEY_DESC[] =
//        "MusicThingyCppBinaryEncodingKey__examples";
//    static constexpr const char LEGACY_DB_ON_DROPBOX_FILENAME[] =
//        "/legacy_db_examples.bin";
//    static constexpr const char LEGACY_DB_LOCAL_COPY_FILENAME[] =
//        "lDbWrkCpy_examples.bin";
//#else
//    static constexpr const char APP_NAME[] = "MusicThingyCpp";
//    static constexpr const char REMOTE_DB_ON_DROPBOX_FILENAME[] = "/my_items.bin";
//    static constexpr char DROPBOX_KEY_DESC[] = "MusicThingyCppDropboxKey";
//    static constexpr char SSL_ENCODE_DB_DESC[] = "MusicThingyCppDbSecret";
//    static constexpr char SSL_ITEM_ENCODING_KEY_DESC[] =
//        "MusicThingyCppBinaryEncodingKey";
//    static constexpr const char LEGACY_DB_ON_DROPBOX_FILENAME[] = "/legacy_db_actual.bin";
//    static constexpr const char LEGACY_DB_LOCAL_COPY_FILENAME[] = "lDbWrkCpy.bin";
//#endif  // ifdef IS_THIS_UNIT_TEST
//private:
//    static std::string combine_paths_and_create_dirs(const std::string &root,
//                                                     const std::string &filename,
//                                                     bool truncate = false) {
//        std::filesystem::path received_dir(root);
//        std::filesystem::path full_path = received_dir / APP_NAME / filename;
//        std::filesystem::create_directories(full_path.parent_path());
//        if (truncate) {
//            if (std::filesystem::exists(full_path)) {
//                std::filesystem::remove(full_path);
//            }
//        }
//        return full_path.make_preferred().generic_string();
//    }
//
//public:
//    /**
//     * Temp database file (local copy of shared database), full path. If path is returned,
//     * it exists. Throws on fail.
//     * @return Example: %APPDATA%/Roaming/<app_name>/db.db
//     * Example 2: /home/domen/.local/share/<app_name>/db.db
//     */
//    static std::string shared_database(bool truncate = false) {
//        return combine_paths_and_create_dirs(sago::getDataHome(), "db.db", truncate);
//    }
//
//    /**
//     * Get file relative to this program's user directory. Create all parent folders if
//     * they don't exist. Truncate existing file if desired.
//     *
//     * @param filename Name of file or path (directory structure).
//     * @param truncate True to truncate file if it exists. Prepares for overwriting.
//     * @return Path (full) that can be used with fstreams and std filesystem.
//     */
//    static std::string file_in_app_root(const std::string &filename,
//                                        bool truncate = false) {
//        return combine_paths_and_create_dirs(sago::getDataHome(), filename, truncate);
//    }
//
//    static std::string local_database() {
//        return combine_paths_and_create_dirs(sago::getDataHome(), "localdb.db");
//    }
//
//    /**
//     * \brief Currently playing song info stored in file. To persist between app launches.
//     * \return Full path to the file.
//     */
//    static std::string playing_song_persistence() {
//        return combine_paths_and_create_dirs(sago::getDataHome(), "cps.json");
//    }
//
//    static std::string example_boost_s() {
//        return combine_paths_and_create_dirs(sago::getDataHome(), "boosts.txt");
//    }
//
//    static std::string example_boost_xml() {
//        return combine_paths_and_create_dirs(sago::getDataHome(), "boosts.xml");
//    }
//
//    static std::string application_config() {
//        return combine_paths_and_create_dirs(sago::getDataHome(), "app.config");
//    }
 public:
  static std::string get_app_folder(bool create_parents = true);
  static std::string file_in_app_folder(const std::string& app_name, const std::string& file_name, bool create_parents = true);
};
}