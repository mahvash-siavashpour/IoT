import datetime
from functools import wraps
from flask import Flask, jsonify, request
from sqlalchemy import Column, Integer, String, ForeignKey, DateTime
from dbmodel import *
import os
from dbmodel import UserSchema, ActivitySchema
from flask_jwt_extended import JWTManager
import jwt


app = Flask(__name__)
basedir = os.path.abspath(os.path.dirname(__file__))
app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///' + os.path.join(basedir, 'offices.db')
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = True
app.config['JWT_SECRET_KEY'] = 'super-secret'
app.config['JSON_SORT_KEYS'] = False

db = SQLAlchemy(app)
jwt_manager = JWTManager(app)


def token_required(f):
    @wraps(f)
    def decorator(*args, **kwargs):
        token = None
        if 'Authorization' in request.headers:
            token = request.headers['Authorization']
            token = token.split('Bearer ')[1]
        if not token:
            return jsonify({'message': 'a valid token is missing'})
        try:
            data = jwt.decode(token, app.config['JWT_SECRET_KEY'], algorithms=["HS256"])
            if 'username' in data.keys():
                current_user = Admin.query.filter_by(username=data['username']).first()
                role = 'admin'

            elif 'servername' in data.keys():
                current_user = LocalServer.query.filter_by(servername=data['servername']).first()
                role = 'localserver'

            else:
                current_user = User.query.filter_by(id=data['id']).first()
                role = 'user'
        except:
            return jsonify({'message': 'token is invalid'})

        return f(current_user, role, *args, **kwargs)

    return decorator


################################
# schema

user_schema = UserSchema()
users_schema = UserSchema(many=True)

activity_schema = ActivitySchema()
activities_schema = ActivitySchema(many=True)
###############################
# DB commands


@app.cli.command('db_create')
def db_create():
    db.create_all()
    print('Database created!')


@app.cli.command('db_drop')
def db_drop():
    db.drop_all()
    print('Database dropped!')


@app.cli.command('db_seed')
def db_seed():

    local_server = LocalServer(servername='local_server',
                               password='12345678')

    super_user = Admin(username='superuser',
                       password='superuser',
                       office_id=1)

    db.session.add(super_user)
    db.session.add(local_server)
    db.session.commit()
    print('DB seeded')


#######################################
# APIs
@app.route("/server/login", methods=['POST'])
def local_server_login():
    servername = request.json['servername']
    password = request.json['password']
    local_server = LocalServer.query.filter_by(servername=servername, password=password).first()
    if local_server:
        access_token = jwt.encode({
            'id': local_server.id,
            'servername': servername,
            # 'exp': datetime.datetime.utcnow() + datetime.timedelta(minutes=60)
        },
            app.config['JWT_SECRET_KEY'])
        return jsonify(message="Login succeeded!", access_token=access_token)
    else:
        return jsonify(message="Bad servername or password"), 401


@app.route('/api/office/register', methods=['POST'])
def add_office():
    lightsOnTime = request.json['lightsOnTime']
    lightsOffTime = request.json['lightsOffTime']

    try:
        new_office = Office(lightsOnTime=lightsOnTime,
                            lightsOffTime=lightsOffTime,)
        db.session.add(new_office)
        db.session.commit()
        return jsonify(message="office added successfully "), 200
    except:
        return jsonify(message="There was an internal issue."), 500


@app.route('/api/admin/register', methods=['POST'])
def admin_register():
    username = request.json['username']
    test = Admin.query.filter_by(username=username).first()
    if test:
        return jsonify(message='That username already exists.'), 409
    else:
        username = request.json['username']
        password = request.json['password']
        office_id = request.json['office_id']
        admin = Admin(username=username, password=password, office_id=office_id)
        db.session.add(admin)
        db.session.commit()
        return jsonify(message="admin created successfully."), 201


@app.route('/api/admin/login', methods=['POST'])
def admin_login():
    username = request.json['username']
    password = request.json['password']

    test = Admin.query.filter_by(username=username, password=password).first()
    if test:
        access_token = jwt.encode({
            'username': username,
            # 'exp': datetime.datetime.utcnow() + datetime.timedelta(minutes=60)
        },
            app.config['JWT_SECRET_KEY'])
        return jsonify(message="Login succeeded!", access_token=access_token)
    else:
        return jsonify(message="Bad username or password"), 401


@app.route('/api/admin/user/register', methods=['POST'])
@token_required
def user_register(current_user, role):
    if role == 'admin':
        rfid_tag = request.json['rfid_tag']
        if User.query.filter_by(rfid_tag=rfid_tag).first():
            return jsonify(message="rfid belongs to user "), 409
        else:
            room_id = request.json['room_id']
            password = request.json['password']
            user = User( password=password, room_id=room_id, office_id=current_user.office_id, rfid_tag=rfid_tag)
            db.session.add(user)
            db.session.commit()
            return jsonify(message="user created successfully."), 201
    else:
        return jsonify(message="you are not authorized as admin"), 401
#


@app.route('/api/admin/activities', methods=['GET'])
@token_required
def get_activities(current_user, role):
    if role == 'admin':
        office_id = current_user.office_id
        if office_id:
            try:
                activities = Activity.query.filter_by(office_id=office_id).all()
                res = activities_schema.dump(activities)
                return jsonify(res)

            except:
                return jsonify(message="There was an internal issue."), 500
        else:
            return jsonify(message="That office does not exist"), 404
    else:
        return jsonify(message="you are not authorized as admin"), 401


@app.route('/api/admin/setlights', methods=['POST'])
@token_required
def set_lights(current_user, role):
    if role == 'admin':
        office = current_user.office
        if office:
            try:
                office.lightsOnTime = request.json['lightsOnTime']
                office.lightsOffTime = request.json['lightsOffTime']
                db.session.commit()
                return jsonify(message="office light time updated successfully"), 202
            except:
                return jsonify(message="There was an internal issue."), 500
        else:
            return jsonify(message="That office does not exist"), 404
    else:
        return jsonify(message="you are not authorized as admin"), 401


@app.route('/api/user/login', methods=['POST'])
def user_login():
    user_id = request.json['id']
    password = request.json['password']
    user = User.query.filter_by(id=user_id, password=password).first()
    if user:
        access_token = jwt.encode({
            'id': user_id,
            'office_id': user.office_id,
            'room_id': user.room_id,
            'light': user.light
            # 'exp': datetime.datetime.utcnow() + datetime.timedelta(minutes=60)
        },
            app.config['JWT_SECRET_KEY'])
        return jsonify(message="Login succeeded!", access_token=access_token)
    else:
        return jsonify(message="Bad id or password"), 401


@app.route('/api/user/<int:id>', methods=["POST"])
@token_required
def user_set_light_percentage(current_user, role, id):
    if role == "user":
        if current_user and current_user.id == id:
            try:
                current_user.light = request.json['light']
                db.session.commit()
                return jsonify(message="room light percentage updated successfully"), 202
            except:
                return jsonify(message="There was an internal issue."), 500
        else:
            return jsonify(message="That user_id does not exist or JWT and user_id is not match"), 404
    else:
        return jsonify(message="you are not authorized as user"), 401


@app.route("/checklight", methods=['POST'])
@token_required
def get_office_light_available(current_user, role):
    if role == 'localserver':
        office_id = request.json["office_id"]
        current_time = request.json["current_time"]
        office = Office.query.filter_by(id=office_id).first()
        if office.lightsOnTime < current_time < office.lightsOffTime:
            return jsonify(message="yes")
        else:
            return jsonify(message="no")
    else:
        return jsonify(message="you are not authorized as a local server"), 401


@app.route("/commitactivity", methods=['POST'])
@token_required
def commit_activity(current_user, role):
    if role == 'localserver':
        rfid_tag = request.json['rfid_tag']
        office_id = request.json['office_id']
        user = User.query.filter_by(rfid_tag=rfid_tag, office_id=office_id).first()
        if not user:
            return jsonify(message="-10")

        else:
            activity_type = request.json['activity_type']
            activity = Activity(user_id=user.id, office_id=user.office_id, type=activity_type)
            db.session.add(activity)
            db.session.commit()
            return jsonify(message=user.light, user_id=user.id, room_id=user.room_id)
    else:
        return jsonify(message="you are not authorized as a local server"), 401

###############################################
#db_model


class Admin(db.Model):
    __tablename__ = 'admin'
    id = Column(Integer, primary_key=True)
    username = Column(String, unique=True)
    password = Column(String)
    office_id = Column(Integer, ForeignKey('office.id'))
    office = db.relationship("Office", backref=db.backref("office"))


class Office(db.Model):
    __tablename__ = 'office'
    id = Column(Integer, primary_key=True)
    lightsOnTime = Column(String)
    lightsOffTime = Column(String)


class User(db.Model):
    __tablename__ = 'user'
    id = Column(Integer, primary_key=True)
    rfid_tag = Column(String)
    password = Column(String)
    light = Column(Integer, default=50)
    office_id = Column(Integer, ForeignKey('office.id'))
    room_id = Column(Integer)



class Activity(db.Model):
    __tablename__ = 'activity'
    activity_id = Column(Integer, primary_key=True)
    user_id = Column(Integer, ForeignKey('user.id'))
    office_id = Column(Integer, ForeignKey('office.id'))
    datatime = Column(DateTime, default=datetime.datetime.now)
    type = Column(String, default='enter')


class LocalServer(db.Model):
    __tablename__ = 'localserver'
    id = Column(Integer, primary_key=True)
    servername = Column(String)
    password = Column(String)


if __name__ == '__main__':
    app.run()
