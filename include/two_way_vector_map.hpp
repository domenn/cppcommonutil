#pragma once


// !! abandoned project

//#include <vector>
//
//namespace d_common {
//
//template <typename __t_first, typename __t_second>
//class TwoWayVectorMap {
//    std::vector<std::pair<__t_first, __t_second>> map_;
//};
//
//}  // namespace d_common
//
//d_reflection::ReflectFieldInfoBase<entity::Song> *entity::Song::field_for_sql_column(
//    const char *col_str, bool allow_empty) {
//    const auto found = std::find_if(
//        field_name_to_sql_table_map_.begin(), field_name_to_sql_table_map_.end(),
//        [col_str](const std::pair<d_reflection::ReflectFieldInfoBase<entity::Song> *,
//                                  const std::string &> &obj) {
//            return obj.second == col_str;
//        });
//    if (found == field_name_to_sql_table_map_.end()) {
//        if (allow_empty) {
//            return nullptr;
//        }
//        LOG_FATAL << "Mapping searched from " << col_str
//                  << " to Song field WAS NOT FOUND! This is not allowed (by "
//                     "parameter allow_empty).";
//        assert(false);
//        throw m_exceptions::BadImplementationException(
//            std::string("Mapping searched from ") + col_str +
//            " to Song field WAS NOT FOUND! This is not allowed (by "
//            "parameter allow_empty).");
//    }
//    return found->first;
//}
//
//std::unique_ptr<entity::Song> entity::Song::from_json(std::string &&input_json) {
//    auto as_json = nlohmann::json::parse(input_json);
//    auto new_song = std::make_unique<Song>();
//    for (const auto &smth : as_json.get<nlohmann::json::object_t>()) {
//        auto key = smth.first;
//        auto val = smth.second;
//        entity::Song::reflect_fields[key]->set_value_from_str(new_song.get(), val);
//    }
//    return new_song;
//}
//
//std::unique_ptr<entity::Song> entity::Song::make_as_uniq_ptr(std::string artist,
//                                                             std::string title,
//                                                             std::string album,
//                                                             std::string filepath) {
//    return std::make_unique<Song>(std::move(artist), std::move(title), std::move(album),
//                                  std::move(filepath));
//}
//
//std::map<std::string, d_reflection::ReflectFieldInfoBase<entity::Song> *>
//    entity::Song::reflect_fields{
//        {entity::Song::artist_field()->name_, entity::Song::artist_field()},
//        D_REFLECT_MAP_LINE(entity::Song::album_field),
//        D_REFLECT_MAP_LINE(entity::Song::filepath_field),
//        D_REFLECT_MAP_LINE(entity::Song::genre_field),
//        D_REFLECT_MAP_LINE(entity::Song::title_field),
//        D_REFLECT_MAP_LINE(entity::Song::date_inserted_field),
//        D_REFLECT_MAP_LINE(entity::Song::date_delete_field),
//        D_REFLECT_MAP_LINE(entity::Song::playcnt_field),
//        D_REFLECT_MAP_LINE(entity::Song::user_tags_field),
//        D_REFLECT_MAP_LINE(entity::Song::latest_state_field)};
//
//// Not all fields are nessecary in this map. But we add them for completeness. With
//// possibly unreleated values.
//const std::vector<
//    std::pair<d_reflection::ReflectFieldInfoBase<entity::Song> *, const std::string &>>
//    entity::Song::field_name_to_sql_table_map_{
//        {artist_field(), SqliteDatabaseCommonConstants::Column::Song::ARTIST},
//        {album_field(), SqliteDatabaseCommonConstants::Column::Song::ALBUM},
//        {title_field(), SqliteDatabaseCommonConstants::Column::Song::TITLE},
//        {filepath_field(), SqliteDatabaseCommonConstants::Column::Song::FILEPATH},
//        {genre_field(), SqliteDatabaseCommonConstants::Column::Song::GENRE},
//        {date_inserted_field(), SqliteDatabaseCommonConstants::Column::Song::DATE_INSERT},
//        {date_delete_field(), SqliteDatabaseCommonConstants::Column::Song::DATE_DELETE},
//        {playcnt_field(), SqliteDatabaseCommonConstants::Column::Song::PLAYCNT},
//        {user_tags_field(), SqliteDatabaseCommonConstants::Column::UserTagsAssignmeng::
//                                USERTAGS_KEY_LEGACY_IMPORT},
//        {latest_state_field(), latest_state_field_.name_}};
//
//const std::string &entity::Song::sql_column_for_field(
//    d_reflection::ReflectFieldInfoBase<entity::Song> *const field_ptr) {
//    auto found = std::find_if(
//        field_name_to_sql_table_map_.begin(), field_name_to_sql_table_map_.end(),
//        [field_ptr](const std::pair<d_reflection::ReflectFieldInfoBase<entity::Song> *,
//                                    const std::string &> &obj) {
//            return obj.first == field_ptr;
//        });
//    assert(found != field_name_to_sql_table_map_.end());
//    return found->second;
//}