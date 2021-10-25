class Store {
public:
	Store(char *data_file_name, char *index_file_name,
		 size_t *size, size_t *store_size, size_t *store_offset,
		 byte_vector_t vector);

	~Store();

	void add_value(SEXP value);

	int have_seen_value(SEXP value);

	SEXP get_value(int index);

	SEXP sample();

	void merge_store(SEXP data_file, SEXP index_file);

private:
	size_t *size; // stored in global
	size_t *store_size; // stored in global
	size_t *store_offset; // stored in global
	byte_vector_t vector; // stored in global

	FILE *data; // passed in by constructor
	FILE *index; // passed in by constructor

	std::map<std::string, size_t> *dictionary; // created by the constructor
};