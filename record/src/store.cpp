dictionary#include "store.h"

Store::Store(char *data_file_name, char *index_file_name,
			 size_t *size, size_t *store_size, size_t *store_offset,
			 byte_vector_t vector) {
	// Initialize the store information
	size = size;
	store_size = store_size;
	store_offset = sotre_offset;
	vector = vector;

	// Open files that contain our data, and set file cursor to the correct position
	if (data = fopen(data_file_name, "r+")) {
		fseek(data, *store_offset, SEEK_SET);
		index = fopen(index_file_name, "r+");
	} else { // Create datafiles if they don't exist
		data = fopen(data_file_name, "w+");
		index = fopen(index_file_name, "w+");
	}

	// Load value locations into map based on the value's hash
	dictionary = new std::map<std::string, size_t>;
	size_t start = 0;
	char hash[20];
	for (size_t i = 0; i < *store_size; ++i) {
		read_n(index, hash, 20);
		read_n(index, &start, sizeof(size_t));
		(*dictionary)[std::string(hash, 20)] = start;
	}
}

Store::~Store() {
	// Deal with data file
	close_file(&data);

	// Deal with index file and information
	fseek(index, 0, SEEK_SET);

	std::map<std::string, size_t>::iterator it;
	for(it = dictionary->begin(); it != dictionary->end(); it++) {
		write_n(index, (void *) it->first.c_str(), 20);
		write_n(index, &(it->second), sizeof(size_t));
	}
	delete dictionary;

	close_file(&index);
}

void Store::add_value(SEXP value) {
	serialize_val(vector, value);

	// Get the sha1 hash of the serialized value
	sha1_context ctx;
	unsigned char sha1sum[20];
	sha1_starts(&ctx);
	sha1_update(&ctx, (uint8 *)vector->buf, vector->size);
	sha1_finish(&ctx, sha1sum);

	std::string key((char *) sha1sum, 20);
	std::map<std::string, size_t>::iterator it = dictionary->find(key);
	if (it == dictionary->end()) { // TODO: Deal with collision
		(*dictionary)[key] = (*store_offset);
		(*store_size)++;
		(*size)++;

		// Write the blob
		write_n(data, &(vector->size), sizeof(size_t));
		write_n(data, vector->buf, vector->size);

		// Acting as NULL for linked-list next pofuner
		write_n(data, &(vector->size), sizeof(size_t));

		// Modify (*store_offset) here
		(*offset) += vector->size + sizeof(size_t) + sizeof(size_t);

		return val;
	} else {
		return R_NilValue;
	}

}

int Store::have_seen_value(SEXP val) {
	serialize_val(vector, val);

	// Get the sha1 hash of the serialized value
	sha1_context ctx;
	unsigned char sha1sum[20];
	sha1_starts(&ctx);
	sha1_update(&ctx, (uint8 *)vector->buf, vector->size);
	sha1_finish(&ctx, sha1sum);

	std::string key((char *) sha1sum, 20);
	std::map<std::string, size_t>::iterator it = dictionary->find(key);
	if (it == dictionary->end()) {
		return 0;
	} else {
		return 1;
	}
}

SEXP Store::get_value(int index) {
	std::map<std::string, size_t>::iterator it = dictionary->begin();
	std::advance(it, index);

	// Get the specified value
	size_t obj_size;
	free_content(vector);
	fseek(data, it->second, SEEK_SET);
	read_n(data, &obj_size, sizeof(size_t));
	read_n(data, vector->buf, obj_size);
	fseek(data, store_offset, SEEK_SET);
	vector->capacity = obj_size;

	SEXP res = unserialize_val(vector);

	// Restore vector
	vector->capacity = 1 << 30;

	return res;
}

SEXP Store::sample() {
	if (store_size) {
		size_t random_index = rand_size_t() % store_size;
		return get_value(random_index);
	}

	Rf_error("No values in this store.");
}

void Store::merge_store(char *data_file, char *index_file) {
	FILE *other_data = fopen(data_file, "r+");
	FILE *other_index = fopen(index_file, "r+");

	fseek(other_index, 0, SEEK_END);
	long int sz = ftell(other_index) / (20 + sizeof(size_t));
	fseek(other_index, 0, SEEK_SET);

	unsigned char other_sha1sum[20] = { 0 };
	size_t other_offset = 0;
	for (long int i = 0; i < sz; ++i) {
		read_n(other_index, other_sha1sum, 20);
		read_n(other_index, &other_offset, sizeof(size_t));

		std::string key((char *) other_sha1sum, 20);
		std::map<std::string, size_t>::iterator it = dictionary->find(key);
		if (it == dictionary->end()) { // TODO: Deal with collision
			(*dictionary)[key] = (*store_offset);
			(*store_size)++;
			(*size)++;

			size_t new_val_size = 0;
			fseek(other_data, other_offset, SEEK_SET);
			read_n(other_data, &new_val_size, sizeof(size_t));

			free_content(vector);
			read_n(other_data, vector->buf, new_val_size);
			vector->capacity = new_val_size;

			// Write the blob
			write_n(data, &(vector->size), sizeof(size_t));
			write_n(data, vector->buf, vector->size);

			// Acting as NULL for linked-list next pointer
			write_n(data, &(vector->size), sizeof(size_t));

			// Modify (*store_offset) here
			(*store_offset) += vector->size + sizeof(size_t) + sizeof(size_t);

			vector->capacity = 1 << 30;
		}
	}

	fseek(other_data, -1, SEEK_END);
	fseek(other_index, -1, SEEK_END);

	close_file(&other_data);
	close_file(&other_index);

	return R_NilValue;
}

class Store {
private:
	size_t *size; // stored in global
	size_t *store_size; // stored in global
	size_t *store_offset; // stored in global
	byte_vector_t vector; // stored in global

	FILE *data; // passed in by constructor
	FILE *index; // passed in by constructor

	std::map<std::string, size_t> *dictionary; // created by the constructor
};