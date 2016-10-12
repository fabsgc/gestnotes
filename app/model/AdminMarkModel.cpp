/*\
 | ------------------------------------------------------
 | @file : AdminTeacherModel.cpp
 | @author : Fabien Beaujean, Luc Lorentz
 | @description : modèle gestion des notes par les administrateurs
 | ------------------------------------------------------
\*/

#include "AdminMarkModel.hpp"

    namespace app
    {
        AdminMarkModel::AdminMarkModel(core::Config &config, core::Database &database, core::Request &request) :
        Model::Model(config, database, request)
        {

        }

        entity::Course AdminMarkModel::getCourseById(std::string id)
        {
        	entity::Course course(mDatabase);
            core::Sql sql(mDatabase);
            dataMap data;

            sql.var("id", id);
            sql.query("SELECT id FROM course WHERE id = :id");
            data = sql.fetch();

            if(data.size() == 1)
            {
                course.hydrate(data["0"]["id"]);
            }

            return course;
        }

        entity::Group AdminMarkModel::getGroupById(std::string id)
        {
        	entity::Group group(mDatabase);
            core::Sql sql(mDatabase);
            dataMap data;

            sql.var("id", id);
            sql.query("SELECT id FROM groups WHERE id = :id");
            data = sql.fetch();

            if(data.size() == 1)
            {
                group.hydrate(data["0"]["id"]);
            }

            return group;
        }

        entity::Student AdminMarkModel::getStudentById(std::string id)
        {
            entity::Student student(mDatabase);
            core::Sql sql(mDatabase);
            dataMap data;

            sql.var("id", id);
            sql.query("SELECT id FROM student WHERE id = :id");
            data = sql.fetch();

            if(data.size() == 1)
                student.hydrate(data["0"]["id"]);

            return student;
        }

        std::vector<entity::Group> AdminMarkModel::getGroups()
        {
        	core::Sql sql(mDatabase);
            dataMap data;
            std::vector<entity::Group> groups;

            sql.query("SELECT id FROM groups ORDER BY name ASC");
            data = sql.fetch();

            for(auto i = data.begin(); i != data.end(); i++)
            {
                entity::Group teacher(mDatabase);
                teacher.hydrate(i->second["id"]);
                groups.push_back(teacher);
            }

            return groups;
        }

        std::vector<entity::Student> AdminMarkModel::getStudentsByGroupAndCourse(std::string group, std::string course)
        {
        	core::Sql sql(mDatabase);
            dataMap data;
            std::vector<entity::Student> students;

            sql.var("group", group);
            sql.var("course", course);
            sql.query("SELECT s.id FROM student s INNER JOIN groups g ON g.id = s.group_id INNER JOIN l_group_course lgc ON lgc.group_id = g.id WHERE s.group_id = :group AND lgc.course_id = :course");
            data = sql.fetch();

            for(auto i = data.begin(); i != data.end(); i++)
            {
                entity::Student student(mDatabase);
                student.hydrate(i->second["id"]);
                students.push_back(student);
            }

            return students;
        }

        std::vector<entity::Course> AdminMarkModel::getCoursesByGroup(std::string group)
        {
        	core::Sql sql(mDatabase);
            dataMap data;
            std::vector<entity::Course> courses;

            sql.var("group", group);
            sql.query("SELECT course_id FROM l_group_course WHERE group_id = :group");
            data = sql.fetch();

            for(auto i = data.begin(); i != data.end(); i++)
            {
                entity::Course course(mDatabase);
                course.hydrate(i->second["course_id"]);
                courses.push_back(course);
            }

            return courses;
        }

        std::vector<entity::Mark> AdminMarkModel::getMarksByCourse(std::string course)
        {
			core::Sql sql(mDatabase);
            dataMap data;
            std::vector<entity::Mark> marks;

            sql.var("course", course);
            sql.query("SELECT id FROM mark WHERE course_id = :course");
            data = sql.fetch();

            for(auto i = data.begin(); i != data.end(); i++)
            {
                entity::Mark mark(mDatabase);
                mark.hydrate(i->second["id"]);
                marks.push_back(mark);
            }

            return marks;
        }

        bool AdminMarkModel::checkGroupCourse(std::string course, std::string group)
        {
        	core::Sql sql(mDatabase);
            dataMap data;

            sql.var("group", group);
            sql.var("course", course);
            sql.query("SELECT id FROM l_group_course WHERE course_id = :course AND group_id = :group");
            data = sql.fetch();

            if(data.size() == 1)
            {
            	return true;
            }

            return false;
        }

        bool AdminMarkModel::markEnabled(entity::Course course)
        {
        	bool markEnabled = true;

            switch (std::stoi(mRequest.get("type"))) {
                case 1:
                    if(course.coeffCe == 0)
                    {
                        markEnabled = false;
                    }
                break;

                case 2:
                    if(course.coeffDe == 0)
                    {
                        markEnabled = false;
                    }
                break;

                case 3:
                    if(course.coeffTp == 0)
                    {
                        markEnabled = false;
                    }
                break;

                case 4:
                    if(course.coeffProject == 0)
                    {
                        markEnabled = false;
                    }
                break;
            }

            return markEnabled;
        }

        bool AdminMarkModel::checkMarkExist(std::string course, std::string student, std::string type)
        {
        	core::Sql sql(mDatabase);
            dataMap data;

            sql.var("course", course);
            sql.var("student", student);
            sql.var("type", type);
            sql.query("SELECT id FROM mark WHERE course_id = :course AND student_id = :student AND type = :type");
            data = sql.fetch();

            if(data.size() == 1)
            {
            	return true;
            }

            return false;
        }

        entity::Mark AdminMarkModel::getMarkExist(std::string course, std::string student, std::string type)
        {
        	core::Sql sql(mDatabase);
            dataMap data;
            entity::Mark mark(mDatabase);

            sql.var("course", course);
            sql.var("student", student);
            sql.var("type", type);
            sql.query("SELECT id FROM mark WHERE course_id = :course AND student_id = :student AND type = :type");
            data = sql.fetch();

            if(data.size() == 1)
            {
            	mark.hydrate(data["0"]["id"]);
            }

            return mark;
        }
    }
