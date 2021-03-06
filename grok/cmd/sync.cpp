# pragma once

# include "fs/path.cpp"

# include "git/branch.cpp"
# include "git/manager.cpp"
# include "git/open.cpp"
# include "git/repository.cpp"

# include "grok/lib/project.cpp"
# include "grok/util/print.cpp"

namespace grok::cmd {

    void sync () {
        grok::lib::project project;

        git::manager::init();

        for (std::string directory : project.configuration().option("registries")) {
            git::repository repository(git::open(directory));

            if (repository.exists()) {
                git::remote origin(repository.remote("origin"));

                origin.fetch();

                git::branch master(repository.branch("master"));
                git::branch origin_master(origin.branch("master"));

                repository.set_head(master);
                repository.merge(master, origin_master);

                grok::util::print("successfully synced registry in '" + directory + "'");
            }
            else {
                grok::util::print("unable to sync registry in '" + directory + "'");
            }
        }

        git::manager::clean_up();
    }
}