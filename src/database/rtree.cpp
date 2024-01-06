#include "rtree.hpp"

#include "db_util.hpp"

Page::Page(int k_factor, std::vector<int> key_bytes, Page* parent_page, BTree* btree) {
    this->k_factor = k_factor;
    this->key_bytes = key_bytes;
    this->parent_page = parent_page;
    this->btree = btree;
}

int insertion_index_search(void* key, std::vector<Key>& keys, std::vector<int>& key_bytes) {
    if (keys.size() == 0) return 0;

    int l = 0;
    int r = keys.size() - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        int comparison_result = compare_keys(keys.at(m).key, key, key_bytes);
        if (comparison_result == 1) {
            l = m + 1;
        } else if (comparison_result == -1) {
            r = m - 1;
        } else {
            return -1;  // no equal keys
        }
    }

    return l;
}

int page_overflow(Page* page, int child_index, int k_factor) {
    Page* overfull_page = page->page_ptrs.at(child_index);
    if (child_index > 0 && page->page_ptrs.at(child_index - 1)->keys.size() < 2 * k_factor) {
        Page* adjacent_page = page->page_ptrs.at(child_index - 1);
        adjacent_page->keys.insert(adjacent_page->keys.end(), page->keys.at(child_index - 1));
        adjacent_page->keys.insert(adjacent_page->keys.end(), overfull_page->keys.begin(), overfull_page->keys.end());
        adjacent_page->page_ptrs.insert(adjacent_page->page_ptrs.end(), overfull_page->page_ptrs.begin(), overfull_page->page_ptrs.end());
        int m = adjacent_page->keys.size() / 2;
        overfull_page->keys = std::vector<Key>(adjacent_page->keys.begin() + m + 1, adjacent_page->keys.end());
        overfull_page->page_ptrs = std::vector<Page*>(adjacent_page->page_ptrs.begin() + m + 2, adjacent_page->page_ptrs.end());
        page->keys.at(child_index - 1) = adjacent_page->keys.at(m);

    } else if (page->page_ptrs.size() > 1 && page->page_ptrs.at(child_index + 1)->keys.size() < 2 * k_factor) {
        Page* adjacent_page = page->page_ptrs.at(child_index + 1);
    } else {
        return -1;
    }
}

int page_split(Page* page, int child_index) { Page* overfull_page = page->page_ptrs.at(child_index); }

int Page::insert_key(void* key, void* data_pointer) {
    int index = insertion_index_search(key, this->keys, this->key_bytes);
    if (index == -1) return -1;
    if (index < this->page_ptrs.size()) {
        int insertion_status = this->page_ptrs.at(index)->insert_key(key, data_pointer);
        if (insertion_status == -1) return -1;
        if (insertion_status == 1) {
            int status = page_overflow(this, index, this->k_factor);
            if (status == -1) page_split(this, index);
            if (this->keys.size() > 2 * this->k_factor) return 1;
        }
    } else {
        this->keys.insert(this->keys.begin() + index, Key{key, data_pointer});
        if (this->keys.size() > 2 * this->k_factor) return 1;
    }
}

int Page::read_key(void* key) {}
int Page::update_key(void* key, void* data_pointer) {}
int Page::delete_key(void* key) {}

BTree::BTree(int k_factor, std::vector<int> key_bytes) {
    this->k_factor = k_factor;
    this->key_bytes = key_bytes;
    this->height = 0;
    this->root_page = new Page(k_factor, this->key_bytes, nullptr, this);
}

int BTree::insert(void* key, void* data_pointer) { return this->root_page->insert_key(key, data_pointer); }
